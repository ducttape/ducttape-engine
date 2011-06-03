#ifndef DUCTTAPE_ENGINE_EVENT_TIMERTICKEVENT
#define DUCTTAPE_ENGINE_EVENT_TIMERTICKEVENT

#include "MessageEvent.hpp"

namespace dt {

class TimerTickEvent : public MessageEvent {
public:
    TimerTickEvent(const std::string& message, uint32_t interval);
    const std::string GetType() const;

    uint32_t GetInterval() const;

private:
    uint32_t mInterval;
};

}

#endif
