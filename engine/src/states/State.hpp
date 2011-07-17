
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_STATES_STATE
#define DUCTTAPE_ENGINE_STATES_STATE

#include "event/Event.hpp"
#include "event/EventListener.hpp"

namespace dt {

/**
  * A class for managing different stages of menu and gameplay.
  * @warning Class mockup only. Documentation suspended.
  * @see StateManager
  */
class State : public EventListener {
public:
    State();
    virtual const std::string GetType() = 0;
    uint32_t GetTypeID();

    virtual void OnEvent(Event& event) = 0;

    void HandleEvent(Event& event);
};

}

#endif
