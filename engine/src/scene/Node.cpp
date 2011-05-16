#include "Node.hpp"

namespace dt {

Node::Node(const std::string &name) {
    mName = name;
}

void Node::AddChild(Node *child) {
    mChildren[child->GetName()] = child;
}

void Node::AddComponent(Component *component) {
    mComponents[component->GetName()] = component;
}

Node* Node::FindChildNode(const std::string &name, bool recursive) {
    if(mChildren.find(name)!=mChildren.end())
        return *(mChildren.find(name)->second);

    if(recursive){
        for(boost::ptr_map<std::string, Node*>::iterator itr = mChildren.begin(); itr != mChildren.end(); itr++) {
            if(itr->first == name)
                return *(itr->second);
            else {
                Node* childNode = (*itr->second)->FindChildNode(name, recursive);
                if(childNode != nullptr)
                    return childNode;
            }
        }
    }
    return nullptr;
}

Component* Node::FindComponent(const std::string &name) {
    if(mComponents.find(name) != mComponents.end())
        return *(mComponents.find(name)->second);
    return nullptr;
}

const std::string& Node::GetName() {
    return mName;
}

}
