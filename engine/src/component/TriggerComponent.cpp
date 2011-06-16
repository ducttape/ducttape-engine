#include "TriggerComponent.hpp"

namespace dt {

TriggerComponent::TriggerComponent(const std::string& name, TriggerComponentListener* custom_listener)
    : Component(name, custom_listener) {
}

void TriggerComponent::HandleEvent(Event* e) {
    if(e->GetType() == "trigger") {

    }
}

void TriggerComponent::OnActivate() {}

void TriggerComponent::OnDeactivate() {}

void TriggerComponent::OnUpdate(float time_diff) {}

}
