#ifndef DUCTTAPE_ENGINE_EVENT_EVENTLISTENER
#define DUCTTAPE_ENGINE_EVENT_EVENTLISTENER

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
    virtual void HandleEvent(Event* e);
};

}

#endif
