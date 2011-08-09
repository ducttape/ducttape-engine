
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include <Scene/StateManager.hpp>

#include <Core/Root.hpp>
#include <Utils/Utils.hpp>

namespace dt {

StateManager::StateManager()
    : mHasNewState(false),
      mPopCount(0) {}

void StateManager::Initialize() {}

void StateManager::Deinitialize() {
    // cleanup all states
    while(mStates.size() > 0) {
        mStates.back()->Deinitialize();
        mStates.pop_back();
    }
}

StateManager* StateManager::Get() {
    return Root::GetInstance().GetStateManager();
}

void StateManager::SetNewState(State* new_state) {
    mNewState = std::shared_ptr<State>(new_state);
    mHasNewState = true;
}

bool StateManager::ShiftStates() {
    // pop old states
    if(mPopCount > 0) {
        // deactivate and pop states
        while(mPopCount > 0 && mStates.size() > 0) {
            mStates.back()->Deinitialize();
            mStates.pop_back();
            --mPopCount;
        }
        if(mStates.size() > 0) {
            mStates.back()->Initialize();
        }
    }

    // add new state
    if(mHasNewState) {
        if(GetCurrentState() != nullptr) {
            GetCurrentState()->Deinitialize();
        }
        mStates.push_back(mNewState);
        GetCurrentState()->Initialize();
        mHasNewState = false;
    }

    return mStates.size() > 0;
}

void StateManager::Pop(uint16_t count) {
    mPopCount += count;
}

State* StateManager::GetCurrentState() {
    if(mStates.size() > 0)
        return mStates.back().get();
    return nullptr;
}

} // namespace dt
