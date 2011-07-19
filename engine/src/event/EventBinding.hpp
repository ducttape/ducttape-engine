
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_EVENT_EVENTBINDING
#define DUCTTAPE_ENGINE_EVENT_EVENTBINDING

#include <memory>

#include "event/Event.hpp"

namespace dt {

/**
  * Binds an Event to call another, more specific event. Usually used
  * to forward raw input.
  */
class EventBinding {
public:
    /**
      * Default constructor.
      */
    EventBinding();

    /**
      * Advanced constructor.
      * @param trigger_type The type of the event that triggers the new event.
      * @param target The new event to be triggered.
      */
    EventBinding(const std::string& trigger_type, Event* target);

    /**
      * Destructor.
      */
    ~EventBinding();

    /**
      * Called when an event occurs that might match the trigger type.
      * @param e The event that occured.
      */
    void TriggerEvent(std::shared_ptr<Event> e);

private:
    std::string mTriggerType;       //!< The type of the event that triggers the new event.
    std::shared_ptr<Event> mTarget; //!< The new event to be triggered.
};

}

#endif
