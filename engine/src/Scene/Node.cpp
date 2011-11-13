
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

Node::Node(const QString& name)
    : mName(name),
      mPosition(Ogre::Vector3::ZERO),
      mScale(Ogre::Vector3(1,1,1)),
      mRotation(Ogre::Quaternion::IDENTITY),
      mParent(nullptr) {

    // auto-generate name
    if(mName == "") {
        mName = "Node-" + Utils::ToString(Utils::AutoId());
    }

    // Generate an uuid for this node.
    mId = Utils::GenerateUUIDRandom();
}

void Node::Initialize() {

    OnInitialize();
}

void Node::Deinitialize() {
    OnDeinitialize();

    // clear all children
    while(mChildren.size() > 0) {
        RemoveChildNode(mChildren.begin()->first);
    }

    // clear all components
    while(mComponents.size() > 0) {
        RemoveComponent(mComponents.begin()->second->GetName());
    }
}

void Node::OnInitialize() {}

void Node::OnDeinitialize() {}

Node* Node::AddChildNode(Node* child) {
    QString key(child->GetName());
    mChildren.insert(key, child);
    mChildren[key].SetParent(this);
    mChildren[key].Initialize();
    return FindChildNode(key, false);
}

Node* Node::FindChildNode(const QString& name, bool recursive) {
    if(mChildren.find(name) != mChildren.end())
        return mChildren.find(name)->second;

    if(recursive){
        for(boost::ptr_map<QString, Node>::iterator itr = mChildren.begin(); itr != mChildren.end(); itr++) {
            if(itr->first == name)
                return itr->second;
            else {
                Node* childNode = itr->second->FindChildNode(name, recursive);
                if(childNode != nullptr)
                    return childNode;
            }
        }
    }
    return nullptr;
}

bool Node::HasComponent(const QString &name) {
    return (mComponents.count(name) > 0);
}

void Node::RemoveChildNode(const QString& name) {
    if(FindChildNode(name, false) != nullptr) {
        FindChildNode(name, false)->Deinitialize(); // destroy recursively
        mChildren.erase(name);
    }
}

void Node::RemoveComponent(const QString& name) {
    if(HasComponent(name)) {
        mComponents[name]->Destroy();
        mComponents.erase(name);
    }
}

const QString& Node::GetName() const {
    return mName;
}

QString Node::GetFullName() const {
    if(mParent == nullptr)
        return GetName();
    else
        return mParent->GetFullName() + "/" + GetName();
}

Ogre::Vector3 Node::GetPosition(Node::RelativeTo rel) const {
    if(rel == PARENT || mParent == nullptr) {
        return mPosition;
    } else {
        return mParent->GetPosition(SCENE) + mParent->GetRotation() * mPosition;
    }
}

void Node::SetPosition(Ogre::Vector3 position, Node::RelativeTo rel) {
    if(mIsUpdatingAfterChange)
        return;

    if(rel == PARENT || mParent == nullptr) {
        mPosition = position;
    } else {
        mPosition = mParent->GetRotation() * position - mParent->GetPosition(SCENE);
    }
    OnUpdate(0);
}

Ogre::Vector3 Node::GetScale(Node::RelativeTo rel) const {
    if(rel == PARENT || mParent == nullptr) {
        return mScale;
    } else {
        Ogre::Vector3 p(mParent->GetScale(SCENE));
        return Ogre::Vector3(p.x * mScale.x, p.y * mScale.y, p.z * mScale.z);
    }
}

void Node::SetScale(Ogre::Vector3 scale, Node::RelativeTo rel) {
    if(mIsUpdatingAfterChange)
        return;

    if(rel == PARENT || mParent == nullptr) {
        mScale = scale;
    } else {
        Ogre::Vector3 p(mParent->GetScale(SCENE));
        mScale = Ogre::Vector3(scale.x / p.x, scale.y / p.y, scale.z / p.z);
    }
    OnUpdate(0);
}

void Node::SetScale(Ogre::Real scale, Node::RelativeTo rel) {
    SetScale(Ogre::Vector3(scale, scale, scale), rel);
}

Ogre::Quaternion Node::GetRotation(Node::RelativeTo rel) const {
    if(rel == PARENT || mParent == nullptr) {
        return mRotation;
    } else {
        return mParent->GetRotation(SCENE) * mRotation;
    }
}

void Node::SetRotation(Ogre::Quaternion rotation, Node::RelativeTo rel) {
    if(mIsUpdatingAfterChange)
        return;

    if(rel == PARENT || mParent == nullptr) {
        mRotation = rotation;
    } else {
        // TODO: implement backward rotation
        mRotation = mParent->GetRotation(SCENE) * (-rotation);
    }
    OnUpdate(0);
}

void Node::SetDirection(Ogre::Vector3 direction, Ogre::Vector3 front_vector) {
    SetRotation(front_vector.getRotationTo(direction, Ogre::Vector3::UNIT_X));
}

void Node::LookAt(Ogre::Vector3 target, Ogre::Vector3 front_vector, RelativeTo rel) {
    SetDirection(target - GetPosition(rel), front_vector);
}

void Node::SetParent(Node* parent) {
    if(parent != nullptr) {
        if(parent->FindChildNode(mName, false) == nullptr) { // we are not already a child of the new parent
            parent->AddChildNode(this);
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
    _UpdateAllComponents(0);
}

Node* Node::GetParent() {
    return mParent;
}

Scene* Node::GetScene() {
    if(_IsScene())
        return (Scene*)this;
    else if(mParent != nullptr)
        return mParent->GetScene();
    else
        return nullptr;
}

void Node::OnUpdate(double time_diff) {
    _UpdateAllChildren(time_diff);
    _UpdateAllComponents(time_diff);
}

void Node::Serialize(IOPacket& packet) {
    packet.Stream(mId, "uuid");
    packet.Stream(mName, "name", mName);
    packet.Stream(mPosition, "position");
    packet.Stream(mScale, "scale", Ogre::Vector3(1,1,1));
    packet.Stream(mRotation, "rotation");
    OnSerialize(packet);

    // Components
    uint32_t count = packet.BeginList(mComponents.size(), "components");

    if(packet.GetDirection() == IOPacket::SERIALIZE) {
        // serialize
        for(auto iter = mComponents.begin(); iter != mComponents.end(); ++iter) {
            packet.BeginObject();
            iter->second->Serialize(packet);
            packet.EndObject();
        }
    } else {
        for(uint32_t i = 0; i < count; ++i) {
            packet.BeginObject();
            std::string type;
            packet.Stream(type, "type", std::string(""));
            Component* c = Serializer::CreateComponent(type);
            c->Serialize(packet);
            AddComponent(c);
            packet.EndObject();
        }
    }
    packet.EndList();

    // Children
    count = packet.BeginList(mChildren.size(), "children");

    if(packet.GetDirection() == IOPacket::SERIALIZE) {
        for(auto iter = mChildren.begin(); iter != mChildren.end(); ++iter) {
            packet.BeginObject();
            iter->second->Serialize(packet);
            packet.EndObject();
        }
    } else {
        for(uint32_t i = 0; i < count; ++i) {
            packet.BeginObject();
            Node* n = new Node;
            n->Serialize(packet);
            AddChildNode(n);
            packet.EndObject();
        }
    }
    packet.EndList();
}

void Node::OnSerialize(IOPacket &packet) {}

void Node::SetPosition(float x, float y, float z, RelativeTo rel) {
    SetPosition(Ogre::Vector3(x,y,z), rel);
}


bool Node::_IsScene() {
    return false;
}

void Node::_UpdateAllComponents(double time_diff) {
    mIsUpdatingAfterChange = (time_diff == 0);

    for(auto iter = mComponents.begin(); iter != mComponents.end(); ++iter) {
        if(iter->second->IsEnabled()) {
            iter->second->OnUpdate(time_diff);
        }
    }

    mIsUpdatingAfterChange = false;
}

void Node::_UpdateAllChildren(double time_diff) {
    mIsUpdatingAfterChange = (time_diff == 0);

    for(auto iter = mChildren.begin(); iter != mChildren.end(); ++iter) {
        iter->second->OnUpdate(time_diff);
    }

    mIsUpdatingAfterChange = false;
}

} // namespace dt
