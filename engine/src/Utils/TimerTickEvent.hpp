
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_EVENT_TIMERTICKEVENT
#define DUCTTAPE_ENGINE_EVENT_TIMERTICKEVENT

#include <Config.hpp>

#include <Event/Event.hpp>
#include <Event/MessageEvent.hpp>

#include <QString>

#include <memory>

namespace dt {

/**
  * The event triggered by a Timer every tick.
  */
class DUCTTAPE_API TimerTickEvent : public MessageEvent {
public:
    /**
      * Advanced constructor.
      * @param message The message the timer was configured with.
      * @param interval The interval of the timer.
      */
    TimerTickEvent(const QString& message, double interval);
    const QString GetType() const;
    std::shared_ptr<Event> Clone() const;

    /**
      * Returns the interval of the timer.
      * @returns The interval of the timer.
      */
    double GetInterval() const;

private:
    double mInterval; //!< The interval the timer was configured with.
};

} // namespace dt

#endif
