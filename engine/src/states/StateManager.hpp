
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_STATES_STATEMANAGER
#define DUCTTAPE_ENGINE_STATES_STATEMANAGER

#include <boost/ptr_container/ptr_vector.hpp>
#include <memory>

#include "State.hpp"
#include "Manager.hpp"

namespace dt {

/**
  * A manager responsible for managing game states.
  * @see State
  */
class StateManager : public Manager {
public:
    /**
      * Default constructor.
      */
    StateManager();

    void Initialize();
    void Deinitialize();

    void SetNewState(State* new_state);
    void PushNewState();
    State* GetCurrentState();

private:
    std::shared_ptr<State> mNewState; //!< The newly created stack to be pushed onto the stack in the next step
    boost::ptr_vector<State> mStates;    //!< The stack of game states
};

}

#endif
