#ifndef DUCTTAPE_ENGINE_EVENT_MESSAGEEVENT
#define DUCTTAPE_ENGINE_EVENT_MESSAGEEVENT

#include "Event.hpp"

namespace dt {

class MessageEvent : public Event {
public:
    MessageEvent(const std::string& message);
    const std::string GetType() const;

    const std::string& GetMessage() const;
private:
    std::string mMessage;
};

}

#endif
