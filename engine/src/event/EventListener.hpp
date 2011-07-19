
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_EVENT_EVENTLISTENER
#define DUCTTAPE_ENGINE_EVENT_EVENTLISTENER

#include <memory>

#include "Event.hpp"

namespace dt {

/**
  * Interface for Event listeners.
  * @see Event
  * @see EventManager
  */
class EventListener {
public:
    /**
      * Pure virtual destructor to make this class abstract.
      */
    virtual ~EventListener() = 0;

    /**
      * Event callback.
      * @param e The Event to handle.
      */
    virtual void HandleEvent(std::shared_ptr<Event> e) = 0;
};

}

#endif
