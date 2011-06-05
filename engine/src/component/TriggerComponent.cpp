#include "TriggerComponent.hpp"

namespace dt {

TriggerComponent::TriggerComponent(TriggerComponentListener* custom_listener)
    : Component<TriggerComponentListener>(custom_listener) {

}

TriggerComponent::TriggerComponent(const std::string& name, TriggerComponentListener* custom_listener)
    : Component<TriggerComponentListener>(name, custom_listener) {
}

void TriggerComponent::HandleEvent(Event* e) {
    if(e->GetType() == "trigger") {

    }
}

}
