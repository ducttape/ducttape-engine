
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_STATES_STATEMANAGER
#define DUCTTAPE_ENGINE_STATES_STATEMANAGER

#include <Config.hpp>

#include <Core/Manager.hpp>
#include <Scene/State.hpp>

#include <cstdint>
#include <memory>
#include <vector>

namespace dt {

/**
  * A manager responsible for managing game states.
  * @see State
  */
class DUCTTAPE_API StateManager : public Manager {
    Q_OBJECT
public:
    /**
      * Default constructor.
      */
    StateManager();

    void Initialize();
    void Deinitialize();

    /**
      * Returns a pointer to the Manager instance.
      * @returns A pointer to the Manager instance.
      */
    static StateManager* Get();

    /**
      * Sets the state to be pushed on the stack in the next frame.
      * @param new_state The new state.
      */
    void SetNewState(State* new_state);

    /**
      * Adds and removes all pending states. Called every frame.
      * @returns Whether any states are left.
      */
    bool ShiftStates();

    /**
      * Removes \c count states from the stack in the next frame.
      * @param count The number of states to remove.
      */
    void Pop(uint16_t count = 1);

    /**
      * Returns the current state.
      * @returns The current state.
      */
    State* GetCurrentState();

private:
    std::shared_ptr<State> mNewState;   //!< The newly created game state to be pushed onto the stack in the next step.
    bool mHasNewState;  //!< Whether a new state has been assigned.
    std::vector<std::shared_ptr<State> > mStates;   //!< The stack of game states.
    uint16_t mPopCount; //!< The number of states to remove in the next frame.
};

} // namespace dt

#endif
