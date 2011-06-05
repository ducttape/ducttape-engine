#include "Node.hpp"

#include "utils/Logger.hpp"
#include "Scene.hpp" // after forward declaration
#include "utils/StringManager.hpp"
#include "Root.hpp"

#include <memory>

namespace dt {

Node::Node(const std::string& name) {
    if(name != "") {
        mName = name;
    } else {
        // generate a name
        // Root::get_mutable_instance().GetIDManager()->GetNextDynamicID();
    }
    mParent = nullptr;
}

void Node::AddChildNode(Node* child) {
    std::string key = child->GetName();
    mChildren[key] = *child;
    mChildren[key].SetParent(this);
}

void Node::AddComponent(Component* component) {
    if(!HasComponent(component->GetName()))
        Root::get_mutable_instance().GetComponentsManager()->AddComponent(this, component);
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
    return Root::get_mutable_instance().GetComponentsManager()->GetNodeOfComponent(name) == mName;
}

void Node::RemoveChildNode(const std::string& name) {
    if(FindChildNode(name, false) != nullptr) {
        mChildren.erase(name);
    }
}

void Node::RemoveComponent(const std::string& name) {
    if(HasComponent(name)) {
        Root::get_mutable_instance().GetComponentsManager()->DestroyComponent(name);
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
    if(rel == PARENT || mParent == nullptr) {
        mPosition = position;
    } else {
        mPosition = position - mParent->GetPosition(SCENE);
    }
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
    if(rel == PARENT || mParent == nullptr) {
        mScale = scale;
    } else {
        Ogre::Vector3 p = mParent->GetScale(SCENE);
        mScale = Ogre::Vector3(scale.x / p.x, scale.y / p.y, scale.z / p.z);
    }
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
    if(rel == PARENT || mParent == nullptr) {
        mRotation = rotation;
    } else {
        Logger::Get().Error("Not implemented: Node::SetRotation relative to SCENE");
        // TODO: implement backward rotation
        // Ogre::Quaternion p = mParent->GetRotation(SCENE);
        mRotation = rotation;
    }
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

bool Node::_IsScene() {
    return false;
}

}
