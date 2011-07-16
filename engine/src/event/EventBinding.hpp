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
    void TriggerEvent(Event* e);

private:
    std::string mTriggerType;       //!< The type of the event that triggers the new event.
    std::shared_ptr<Event> mTarget; //!< The new event to be triggered.
};

}

#endif
