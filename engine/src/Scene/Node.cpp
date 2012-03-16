
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include <Scene/Node.hpp>

#include <Utils/Utils.hpp>
#include <Scene/Scene.hpp>
#include <Scene/Serializer.hpp>

namespace dt {

Node::Node(const QString name)
    : mName(name),
      mPosition(Ogre::Vector3::ZERO),
      mScale(Ogre::Vector3(1,1,1)),
      mRotation(Ogre::Quaternion::IDENTITY),
      mParent(nullptr),
      mDeathMark(false),
      mIsEnabled(true) {

    // auto-generate name
    if(mName == "") {
        mName = "Node-" + Utils::toString(Utils::autoId());
    }

    mId = QUuid::createUuid();
}

void Node::initialize() {
    onInitialize();
}

void Node::deinitialize() {
    onDeinitialize();

    // clear all children
    while(mChildren.size() > 0) {
        removeChildNode(mChildren.begin()->first);
    }

    // clear all components
    while(mComponents.size() > 0) {
        removeComponent(mComponents.begin()->second->getName());
    }
}

void Node::onInitialize() {}

void Node::onDeinitialize() {}

Node::NodeSP Node::addChildNode(Node* child) {
    if(child != nullptr) {
        QString key(child->getName());
        NodeSP child_sp(child);
        mChildren.insert(std::make_pair(key, child_sp));
        child_sp->setParent(this);
        child_sp->initialize();

        if(!mIsEnabled)
            child->disable();

        return findChildNode(key, false);
    }
    else {
        return nullptr;
    }
}

Node::NodeSP Node::findChildNode(const QString name, bool recursive) {
    if(mChildren.find(name) != mChildren.end())
        return mChildren.find(name)->second;

    if(recursive){
        for(std::map<QString, NodeSP>::iterator itr = mChildren.begin(); itr != mChildren.end(); itr++) {
            if(itr->first == name)
                return itr->second;
            else {
                const NodeSP& childNode = itr->second->findChildNode(name, recursive);
                if(childNode)
                    return childNode;
            }
        }
    }
    return NodeSP();
}

bool Node::hasComponent(const QString name) {
    return (mComponents.count(name) > 0);
}

void Node::removeChildNode(const QString name) {
    if(findChildNode(name, false) != nullptr) {
        findChildNode(name, false)->deinitialize(); // destroy recursively
        mChildren.erase(name);
    }
}

void Node::removeComponent(const QString name) {
    if(hasComponent(name)) {
        mComponents[name]->deinitialize();
        mComponents.erase(name);
    }
}

const QString Node::getName() const {
    return mName;
}

QString Node::getFullName() const {
    if(mParent == nullptr)
        return getName();
    else
        return mParent->getFullName() + "/" + getName();
}

Ogre::Vector3 Node::getPosition(Node::RelativeTo rel) const {
    if(rel == PARENT || mParent == nullptr) {
        return mPosition;
    } else {
        return mParent->getPosition(SCENE) + mParent->getRotation() * mPosition;
    }
}

void Node::setPosition(Ogre::Vector3 position, Node::RelativeTo rel) {
    if(mIsUpdatingAfterChange || !mIsEnabled)
        return;

    if(rel == PARENT || mParent == nullptr) {
        mPosition = position;
    } else {
        mPosition = mParent->getRotation() * position - mParent->getPosition(SCENE);
    }
    onUpdate(0);
}

Ogre::Vector3 Node::getScale(Node::RelativeTo rel) const {
    if(rel == PARENT || mParent == nullptr) {
        return mScale;
    } else {
        Ogre::Vector3 p(mParent->getScale(SCENE));
        return Ogre::Vector3(p.x * mScale.x, p.y * mScale.y, p.z * mScale.z);
    }
}

void Node::setScale(Ogre::Vector3 scale, Node::RelativeTo rel) {
    if(mIsUpdatingAfterChange || !mIsEnabled)
        return;

    if(rel == PARENT || mParent == nullptr) {
        mScale = scale;
    } else {
        Ogre::Vector3 p(mParent->getScale(SCENE));
        mScale = Ogre::Vector3(scale.x / p.x, scale.y / p.y, scale.z / p.z);
    }
    onUpdate(0);
}

void Node::setScale(Ogre::Real scale, Node::RelativeTo rel) {
    setScale(Ogre::Vector3(scale, scale, scale), rel);
}

Ogre::Quaternion Node::getRotation(Node::RelativeTo rel) const {
    if(rel == PARENT || mParent == nullptr) {
        return mRotation;
    } else {
        return mParent->getRotation(SCENE) * mRotation;
    }
}

void Node::setRotation(Ogre::Quaternion rotation, Node::RelativeTo rel) {
    if(mIsUpdatingAfterChange || !mIsEnabled)
        return;

    if(rel == PARENT || mParent == nullptr) {
        mRotation = rotation;
    } else {
        // TODO: implement backward rotation
        mRotation = mParent->getRotation(SCENE) * (-rotation);
    }
    onUpdate(0);
}

void Node::setDirection(Ogre::Vector3 direction, Ogre::Vector3 front_vector) {
    setRotation(front_vector.getRotationTo(direction, Ogre::Vector3::UNIT_X));
}

void Node::lookAt(Ogre::Vector3 target, Ogre::Vector3 front_vector, RelativeTo rel) {
    setDirection(target - getPosition(rel), front_vector);
}

void Node::setParent(Node* parent) {
    if(parent != nullptr) {
        if(parent->findChildNode(mName, false).get() == nullptr) { // we are not already a child of the new parent
            if(mParent != nullptr) {                         // Remove it from its original parent.
                auto iter = mParent->mChildren.find(mName);
                parent->mChildren.insert(std::make_pair(mName, iter->second));
                mParent->mChildren.erase(iter);
                mParent = parent;
            }
            else {
                parent->addChildNode(this);
            }
            return;
        }
    }
    /*
    if(mParent != parent && mParent != nullptr) {
        // new parent
        mParent->RemoveChildNode(mName);
    } */

    mParent = parent;

    // the absolute position might have changed!
    _updateAllComponents(0);
}

Node* Node::getParent() {
    return mParent;
}

Scene* Node::getScene() {
    if(_isScene())
        return (Scene*)this;
    else if(mParent != nullptr)
        return mParent->getScene();
    else
        return nullptr;
}

void Node::onUpdate(double time_diff) {
    if(mIsEnabled) {
        _updateAllChildren(time_diff);
        _updateAllComponents(time_diff);
    }
}

void Node::serialize(IOPacket& packet) {
    packet.stream(mId, "uuid");
    packet.stream(mName, "name", mName);
    packet.stream(mPosition, "position");
    packet.stream(mScale, "scale", Ogre::Vector3(1,1,1));
    packet.stream(mRotation, "rotation");
    packet.stream(mIsEnabled, "enabled");
    onSerialize(packet);

    // Components
    uint32_t count = packet.beginList(mComponents.size(), "components");

    if(packet.getDirection() == IOPacket::SERIALIZE) {
        // serialize
        for(auto iter = mComponents.begin(); iter != mComponents.end(); ++iter) {
            packet.beginObject();
            iter->second->serialize(packet);
            packet.endObject();
        }
    } else {
        for(uint32_t i = 0; i < count; ++i) {
            packet.beginObject();
            std::string type;
            packet.stream(type, "type", std::string(""));
            Component* c = Serializer::createComponent(type);
            c->serialize(packet);
            addComponent(c);
            packet.endObject();
        }
    }
    packet.endList();

    // Children
    count = packet.beginList(mChildren.size(), "children");

    if(packet.getDirection() == IOPacket::SERIALIZE) {
        for(auto iter = mChildren.begin(); iter != mChildren.end(); ++iter) {
            packet.beginObject();
            iter->second->serialize(packet);
            packet.endObject();
        }
    } else {
        for(uint32_t i = 0; i < count; ++i) {
            packet.beginObject();
            Node* n = new Node;
            n->serialize(packet);
            addChildNode(n);
            packet.endObject();
        }
    }
    packet.endList();
}

void Node::onSerialize(IOPacket &packet) {}

void Node::setPosition(float x, float y, float z, RelativeTo rel) {
    setPosition(Ogre::Vector3(x,y,z), rel);
}


bool Node::_isScene() {
    return false;
}

void Node::_updateAllComponents(double time_diff) {
    mIsUpdatingAfterChange = (time_diff == 0);

    for(auto iter = mComponents.begin(); iter != mComponents.end(); ++iter) {
        if(iter->second->isEnabled()) {
            iter->second->onUpdate(time_diff);
        }
    }

    mIsUpdatingAfterChange = false;
}

void Node::_updateAllChildren(double time_diff) {
    mIsUpdatingAfterChange = (time_diff == 0);

    for(auto iter = mChildren.begin(); iter != mChildren.end(); ++iter) {
        if(iter->second->mDeathMark) {
            //Kill it if the death mark is set.
            Node* node = iter->second.get();
            iter--;
            QString name = node->getName();
            removeChildNode(name);
        }
        else {
            iter->second->onUpdate(time_diff);
        }
    }

    mIsUpdatingAfterChange = false;
}

void Node::kill() {
    if(mIsEnabled)
        mDeathMark = true;
}

bool Node::isEnabled() {
    return mIsEnabled;
}

void Node::enable() {
    if(mParent == nullptr || mParent->isEnabled()) {
        mIsEnabled = true;
        
        for(auto iter = mChildren.begin(); iter != mChildren.end(); ++iter) {
            iter->second->enable();
        }

        for(auto iter = mComponents.begin(); iter != mComponents.end(); ++iter) {
            iter->second->enable();
        }

        onEnable();
    }
}

void Node::disable() {
    if(mIsEnabled) {
        mIsEnabled = false;

        for(auto iter = mChildren.begin(); iter != mChildren.end(); ++iter) {
            iter->second->disable();
        }

        for(auto iter = mComponents.begin(); iter != mComponents.end(); ++iter) {
            iter->second->disable();
        }

        onDisable();
    }
}

void Node::onEnable() {}

void Node::onDisable() {}

} // namespace dt
