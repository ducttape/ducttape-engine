#include "ComponentsManager.hpp"

#include "scene/Node.hpp"

namespace dt {

ComponentsManager::ComponentsManager() {}

ComponentsManager::~ComponentsManager() {}

void ComponentsManager::Initialize() {}

void ComponentsManager::Deinitialize() {
    for(std::pair<std::string, std::shared_ptr<Component> > set: mComponents) {
        set.second->OnDeactivate();
    }

    // destroy all components
    mComponents.clear();
}

void ComponentsManager::AddComponent(Node* node, Component* component) {
    if(component != nullptr && FindComponent(component->GetName()) == nullptr) {
        auto ptr = std::shared_ptr<Component>(component);
        ptr->SetNode(node);
        ptr->OnActivate();
        mComponents.insert(std::pair<std::string, std::shared_ptr<Component> >(
                               node->GetName(), ptr));
    }
}

Component* ComponentsManager::FindComponent(const std::string& name) {
    for(std::pair<std::string, std::shared_ptr<Component> > set: mComponents) {
        if(set.second->GetName() == name)
            return set.second.get();
    }
    return nullptr;
}

const std::string& ComponentsManager::GetNodeOfComponent(const std::string& name) {
    Component* c = FindComponent(name);
    if(c == nullptr)
        return "";
    else
        return c->GetName();
}

std::vector<std::shared_ptr<Component> > ComponentsManager::GetComponentsOfNode(const std::string& node_name) {
    std::vector<std::shared_ptr<Component> > result;
    for(std::pair<std::string, std::shared_ptr<Component> > set: mComponents) {
        if(set.first == node_name)
            result.push_back(set.second);
    }
    return result;
}

void ComponentsManager::DestroyComponent(const std::string& name) {
    for(auto iter = mComponents.begin(); iter != mComponents.end(); ++iter) {
        if(iter->second->GetName() == name) {
            iter = mComponents.erase(iter);
        }
    }
}

}

// std::shared_ptr<Component>
