#include "ComponentsManager.hpp"

namespace dt {

ComponentsManager::ComponentsManager() {}

ComponentsManager::~ComponentsManager() {}

void ComponentsManager::Initialize() {}

void ComponentsManager::Deinitialize() {
    // destroy all components
    mComponents.clear();
}

void ComponentsManager::AddComponent(const std::string& node_name, Component<ComponentListener>* component) {
    if(component != nullptr && FindComponent(component->GetName()) == nullptr) {
        mComponents.insert(std::pair<std::string, std::shared_ptr<Component<ComponentListener>>>(
                               node_name, std::shared_ptr<Component<ComponentListener>>(component)));
    }
}

Component<ComponentListener>* ComponentsManager::FindComponent(const std::string& name) {
    for(std::pair<std::string, std::shared_ptr<Component<ComponentListener> > > set: mComponents) {
        if(set.second->GetName() == name)
            return set.second.get();
    }
    return nullptr;
}

const std::string& ComponentsManager::GetNodeOfComponent(const std::string& name) {
    Component<ComponentListener>* c = FindComponent(name);
    if(c == nullptr)
        return "";
    else
        return c->GetName();
}

std::vector<std::shared_ptr<Component<ComponentListener> > > ComponentsManager::GetComponentsOfNode(const std::string& node_name) {
    std::vector<std::shared_ptr<Component<ComponentListener> > > result;
    for(std::pair<std::string, std::shared_ptr<Component<ComponentListener> > > set: mComponents) {
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

// std::shared_ptr<Component<ComponentListener> >
