#ifndef DUCTTAPE_ENGINE_EVENT_EVENTLISTENER
#define DUCTTAPE_ENGINE_EVENT_EVENTLISTENER

#include "Event.hpp"

namespace dt {

class EventListener {
public:
    virtual void HandleEvent(Event* e);
};

}

#endif
