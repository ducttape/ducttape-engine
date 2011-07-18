
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_EVENT_TIMER
#define DUCTTAPE_ENGINE_EVENT_TIMER

#include <memory>

#include <boost/signals2.hpp>

#include <SFML/System/Thread.hpp>

#include "TimerTickEvent.hpp"
#include "BeginFrameEvent.hpp"
#include "EventListener.hpp"

namespace dt {

/**
  * A timer to send Tick events in regular intervals.
  */
class Timer : public EventListener {
public:
    /**
      * Advanced constructor.
      * @param message The message to send with the TimerTickEvent.
      * @param interval The interval to wait between 2 ticks.
      * @param repeat Whether the timer should proceed to tick after the first tick.
      * @param threaded Whether the timer should be started in a separate thread or just rely on the BeginFrameEvent.
      * @param use_events Whether the timer should create a TimerTickEvent every tick or just call the signal.
      */
    Timer(const std::string& message, uint32_t interval, bool repeat = true, bool threaded = false, bool use_events = true);

    void HandleEvent(Event* e);

    /**
      * Triggers the tick event and resets the timer.
      */
    void TriggerTickEvent();

    /**
      * Returns the Interval.
      * @returns The interval of the timer.
      */
    uint32_t GetInterval() const;

    /**
      * Returns the message to send with the TimerTickEvent.
      * @returns The message to send with the TimerTickEvent.
      */
    const std::string& GetMessageEvent() const;

    /**
      * Stops the timer.
      */
    void Stop();

    /**
      * Binds a slot to the signal.
      */
    boost::signals2::connection BindSlot(boost::function<void (const std::string&)> slot);

private:
    /**
      * Private method. Runs the timer in a thread.
      */
    void _RunThread();

    /**
      * Private method. The actual thread method.
      */
    static void _ThreadFunction(void* user_data);

    std::shared_ptr<sf::Thread> mThread;    //!< The sf::Thread the timer uses in threaded mode.
    boost::signals2::signal<void (const std::string&)> mTickSignal;   //!< The signal to call on tick.
    std::string mMessage;                   //!< The message to send with the TimerTickEvent.
    uint32_t mInterval;                     //!< The timer interval.
    bool mRepeat;                           //!< Whether the timer should proceed to tick after the first tick.
    bool mThreaded;                         //!< Whether the timer runs threaded or not.
    bool mUseEvents;                        //!< Whether the timer should create a TimerTickEvent every tick or just call the signal.

    uint32_t mTimeLeft; //!< The time left until the next tick. Only used in non-threaded mode.
};

}

#endif
