#include "StateManager.hpp"

namespace dt {

StateManager::StateManager() {}

void StateManager::SetNewState(State* new_state) {
    mNewState = std::shared_ptr<State>(new_state);
}

void StateManager::PushNewState() {
    mStates.push_back(mNewState.get());
    mNewState = std::shared_ptr<State>();
}

State* StateManager::GetCurrentState() {
    if(mStates.size() > 0)
        return &(mStates.back());
    return nullptr;
}

void StateManager::HandleEvent(Event& event) {
    for(State& s: mStates) {
        s.HandleEvent(event);
    }
}

}
