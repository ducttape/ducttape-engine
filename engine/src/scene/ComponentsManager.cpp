#include "ComponentsManager.hpp"

namespace dt {

ComponentsManager::ComponentsManager() {}

ComponentsManager::~ComponentsManager() {}

void ComponentsManager::Initialize() {}

void ComponentsManager::Deinitialize() {
    for(Component<ComponentListener>& c: mComponents) {
        DestroyComponent(c.GetName());
    }
}

void ComponentsManager::AddComponent(Component<ComponentListener>* component) {
    if(component != nullptr && FindComponent(component->GetName()) == nullptr) {
        mComponents.push_back(component);
    }
}

Component<ComponentListener>* ComponentsManager::FindComponent(const std::string& name) {
    for(Component<ComponentListener>& c: mComponents) {
        if(c.GetName() == name)
            return &c;
    }
    return nullptr;
}

void ComponentsManager::DestroyComponent(const std::string& name) {
    mComponents.erase_if(boost::bind(&Component<ComponentListener>::GetName, _1) == name);
}

}
