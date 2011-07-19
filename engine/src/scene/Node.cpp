
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include <memory>

#ifdef COMPILER_MSVC
#include <boost/foreach.hpp>
#endif

#include "Node.hpp"
#include "Root.hpp"

#include "utils/Logger.hpp"
#include "Scene.hpp" // after forward declaration
#include "utils/StringManager.hpp"

namespace dt {

Node::Node(const std::string& name) {
    uint32_t new_id;

    if(name == "") {
        new_id = Root::get_mutable_instance().GetStringManager()->GetNextAutoId();
        mName = "Node-"+tostr(new_id);
    } else {
        mName = name;
    }

    mParent = nullptr;

    mPosition = Ogre::Vector3::ZERO;
    mScale = Ogre::Vector3(1,1,1);
    mRotation = Ogre::Quaternion::IDENTITY;
}

void Node::AddChildNode(Node* child) {
    std::string key = child->GetName();
    mChildren.insert(key, child);
    mChildren[key].SetParent(this);
}

void Node::AddComponent(Component* component) {
    const std::string& cname = component->GetName();
    if(!HasComponent(cname)) {
        std::shared_ptr<Component> ptr(component);
        ptr->SetNode(this);
        ptr->Create();
        mComponents.insert(std::pair<std::string, std::shared_ptr<Component> >(cname, ptr));

        _UpdateAllComponents(0);
    }
}

Node* Node::FindChildNode(const std::string& name, bool recursive) {
    if(mChildren.find(name)!=mChildren.end())
        return mChildren.find(name)->second;

    if(recursive){
        for(boost::ptr_map<std::string, Node>::iterator itr = mChildren.begin(); itr != mChildren.end(); itr++) {
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

bool Node::HasComponent(const std::string &name) {
    return (mComponents.count(name) > 0);
}

void Node::RemoveChildNode(const std::string& name) {
    if(FindChildNode(name, false) != nullptr) {
        mChildren.erase(name);
    }
}

void Node::RemoveComponent(const std::string& name) {
    if(HasComponent(name)) {
        mComponents[name]->Destroy();
        mComponents[name]->SetNode(nullptr);
        mComponents.erase(name);
    }
}

const std::string& Node::GetName() const {
    return mName;
}

Ogre::Vector3 Node::GetPosition(Node::RelativeTo rel) const {
    if(rel == PARENT || mParent == nullptr) {
        return mPosition;
    } else {
        return mParent->GetPosition(SCENE) + mPosition;
    }
}

void Node::SetPosition(Ogre::Vector3 position, Node::RelativeTo rel) {
    if(mIsUpdatingAfterChange)
        return;

    if(rel == PARENT || mParent == nullptr) {
        mPosition = position;
    } else {
        mPosition = position - mParent->GetPosition(SCENE);
    }
    OnUpdate(0);
}

Ogre::Vector3 Node::GetScale(Node::RelativeTo rel) const {
    if(rel == PARENT || mParent == nullptr) {
        return mScale;
    } else {
        Ogre::Vector3 p = mParent->GetScale(SCENE);
        return Ogre::Vector3(p.x * mScale.x, p.y * mScale.y, p.z * mScale.z);
    }
}

void Node::SetScale(Ogre::Vector3 scale, Node::RelativeTo rel) {
    if(mIsUpdatingAfterChange)
        return;

    if(rel == PARENT || mParent == nullptr) {
        mScale = scale;
    } else {
        Ogre::Vector3 p = mParent->GetScale(SCENE);
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
        // Logger::Get().Error("Not implemented: Node::GetRotation relative to SCENE");
        return mParent->GetRotation(SCENE) * mRotation;
    }
}

void Node::SetRotation(Ogre::Quaternion rotation, Node::RelativeTo rel) {
    if(mIsUpdatingAfterChange)
        return;

    if(rel == PARENT || mParent == nullptr) {
        mRotation = rotation;
    } else {
        Logger::Get().Error("Not implemented: Node::SetRotation relative to SCENE");
        // TODO: implement backward rotation
        // Ogre::Quaternion p = mParent->GetRotation(SCENE);
        mRotation = rotation;
    }
    OnUpdate(0);
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

bool Node::_IsScene() {
    return false;
}

void Node::_UpdateAllComponents(double time_diff) {
    mIsUpdatingAfterChange = (time_diff == 0);
#ifdef COMPILER_MSVC
    typedef std::pair<std::string, std::shared_ptr<Component> > pair_type;
    BOOST_FOREACH(pair_type pair, mComponents) {
#else
    for(std::pair<std::string, std::shared_ptr<Component> > pair: mComponents) {
#endif
        pair.second->OnUpdate(time_diff);
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

}
