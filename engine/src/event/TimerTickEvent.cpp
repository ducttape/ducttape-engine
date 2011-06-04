#include "TimerTickEvent.hpp"

namespace dt {

TimerTickEvent::TimerTickEvent(const std::string& message, uint32_t interval)
    : MessageEvent(message) {
    mInterval = interval;
}

const std::string TimerTickEvent::GetType() const {
    return "DT_TIMERTICKEVENT";
}

Event* TimerTickEvent::Clone() const {
    return new TimerTickEvent(mMessage, mInterval);
}

uint32_t TimerTickEvent::GetInterval() const {
    return mInterval;
}

}
