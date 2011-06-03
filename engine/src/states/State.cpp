#include "State.hpp"

#include "Root.hpp"

namespace dt {

State::State() {}

uint32_t State::GetTypeID() {
    return Root::get_mutable_instance().GetStringManager()->Get(GetType());
}

void State::Update(float time_delta) {
    // TODO: update scenes

    OnUpdate(time_delta);
}

void State::HandleEvent(Event& event) {
    // TODO: forward to scenes

    OnEvent(event);
}

}
