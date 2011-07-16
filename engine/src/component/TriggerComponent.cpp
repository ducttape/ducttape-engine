#include "TriggerComponent.hpp"

namespace dt {

TriggerComponent::TriggerComponent(const std::string& name)
    : Component(name) {
}

void TriggerComponent::HandleEvent(Event* e) {
    if(e->GetType() == "trigger") {

    }
}

void TriggerComponent::OnCreate() {}

void TriggerComponent::OnDestroy() {}

void TriggerComponent::OnUpdate(float time_diff) {}

}
