#ifndef DUCTTAPE_ENGINE_EVENT_BEGINFRAMEEVENT
#define DUCTTAPE_ENGINE_EVENT_BEGINFRAMEEVENT

#include "Event.hpp"

namespace dt {

class BeginFrameEvent : public Event {
public:
    BeginFrameEvent(uint32_t frame_time);
    const std::string GetType() const;
    Event* Clone() const;
    uint32_t GetFrameTime() const;

private:
    uint32_t mFrameTime;
};

}

#endif
