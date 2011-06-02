#include "ComponentsManager.hpp"

namespace dt {

ComponentsManager::ComponentsManager() {}

ComponentsManager::~ComponentsManager() {}

void ComponentsManager::Initialize() {}

void ComponentsManager::Deinitialize() {
    for(Component& c: mComponents) {
        DestroyComponent(c.GetName());
    }
}

void ComponentsManager::AddComponent(Component* component) {
    if(component != nullptr && FindComponent(component->GetName()) == nullptr) {
        mComponents.push_back(component);
    }
}

Component* ComponentsManager::FindComponent(const std::string& name) {
    for(Component& c: mComponents) {
        if(c.GetName() == name)
            return &c;
    }
    return nullptr;
}

void ComponentsManager::DestroyComponent(const std::string& name) {
    mComponents.erase_if(boost::bind(&Component::GetName, _1) == name);
}

}
