
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

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
