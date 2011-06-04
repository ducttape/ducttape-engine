#ifndef DUCTTAPE_ENGINE_EVENT_EVENTBINDING
#define DUCTTAPE_ENGINE_EVENT_EVENTBINDING

#include <memory>

#include "event/Event.hpp"

namespace dt {

class EventBinding {
public:
    EventBinding();
    EventBinding(const std::string& trigger_type, Event* target);
    ~EventBinding();
    void TriggerEvent(Event* e);

private:
    std::string mTriggerType;
    std::shared_ptr<Event> mTarget;
};

}

#endif
