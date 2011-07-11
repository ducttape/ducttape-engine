#ifndef DUCTTAPE_ENGINE_EVENT_MESSAGEEVENT
#define DUCTTAPE_ENGINE_EVENT_MESSAGEEVENT

#include "Event.hpp"

namespace dt {

/**
  * Event for sending generic string messages through the event system.
  */
class MessageEvent : public Event {
public:
    /**
      * Advanced constructor.
      * @param message The message for this Event.
      */
    MessageEvent(const std::string& message);
    const std::string GetType() const;

    /**
      * Returns the message of this Event.
      * @returns The message of the Event.
      */
    const std::string& GetMessageEvent() const;
protected:
    std::string mMessage;   //!< The message of this Event.
};

}

#endif
