#ifndef DUCTTAPE_ENGINE_STATES_STATEMANAGER
#define DUCTTAPE_ENGINE_STATES_STATEMANAGER

#include <boost/ptr_container/ptr_vector.hpp>
#include <memory>

#include "State.hpp"

namespace dt {

/**
  * A manager responsible for managing game states.
  * @see State
  */
class StateManager {
public:
    /**
      * Default constructor.
      */
    StateManager();

    void SetNewState(State* new_state);
    void PushNewState();
    State* GetCurrentState();

    void HandleEvent(Event& event);
private:
    std::shared_ptr<State> mNewState; //!< The newly created stack to be pushed onto the stack in the next step
    boost::ptr_vector<State> mStates;    //!< The stack of game states
};

}

#endif
