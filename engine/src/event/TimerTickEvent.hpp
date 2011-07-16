#ifndef DUCTTAPE_ENGINE_EVENT_TIMERTICKEVENT
#define DUCTTAPE_ENGINE_EVENT_TIMERTICKEVENT

#include "MessageEvent.hpp"

namespace dt {

/**
  * The event triggered by a Timer every tick.
  */
class TimerTickEvent : public MessageEvent {
public:
    /**
      * Advanced constructor.
      * @param message The message the timer was configured with.
      * @param interval The interval of the timer.
      */
    TimerTickEvent(const std::string& message, uint32_t interval);
    const std::string GetType() const;
    Event* Clone() const;

    /**
      * Returns the interval of the timer.
      * @returns The interval of the timer.
      */
    uint32_t GetInterval() const;

private:
    uint32_t mInterval; //!< The interval the timer was configured with.
};

}

#endif
