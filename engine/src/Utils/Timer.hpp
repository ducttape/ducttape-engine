
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_EVENT_TIMER
#define DUCTTAPE_ENGINE_EVENT_TIMER

#include <Config.hpp>

#include <SFML/System/Sleep.hpp>
#include <SFML/System/Thread.hpp>

#include <QObject>
#include <QString>

#include <memory>

namespace dt {

/**
  * A timer to send Tick events in regular intervals.
  */
class DUCTTAPE_API Timer : public QObject {
    Q_OBJECT

public:
    /**
      * Advanced constructor.
      * @param message The message to send with the TimerTickEvent.
      * @param interval The interval to wait between 2 ticks.
      * @param repeat Whether the timer should proceed to tick after the first tick.
      * @param threaded Whether the timer should be started in a separate thread or just rely on the BeginFrameEvent.
      */
    Timer(const QString& message, double interval, bool repeat = true,
          bool threaded = false);

    /**
      * Triggers the tick event and resets the timer.
      */
    void TriggerTickEvent();

    /**
      * Returns the Interval.
      * @returns The interval of the timer.
      */
    double GetInterval() const;

    /**
      * Returns the message to send with the TimerTickEvent.
      * @returns The message to send with the TimerTickEvent.
      */
    const QString& GetMessageEvent() const;

    /**
      * Stops the timer.
      */
    void Stop();

public slots:
    /**
     * Manually triggers a timer tick. This is mainly used for debugging
     * purposes.
     */
    void TriggerTick();
    
    /**
     * Update the time left, only used in non-threaded mode.
     * @param frame_time The duration of the frame. 
     */
    void UpdateTimeLeft(const double& frame_time);

signals:
    void TimerTicked(const QString& message, double interval);

    void TimerStoped();
private:
    /**
      * Private method. Runs the timer in a thread.
      */
    void _RunThread();

    /**
      * Private method. The actual thread method.
      * @param user_data Data that is being passed in by the sf::Thread (void* to Timer instance).
      */
    static void _ThreadFunction(void* user_data);

    std::shared_ptr<sf::Thread> mThread;    //!< The sf::Thread the timer uses in threaded mode.
    QString mMessage;                       //!< The message to send with the TimerTickEvent.
    double mInterval;                       //!< The timer interval, in seconds.
    bool mRepeat;                           //!< Whether the timer should proceed to tick after the first tick.
    bool mThreaded;                         //!< Whether the timer runs threaded or not.

    double mTimeLeft; //!< The time left until the next tick. Only used in non-threaded mode.
};

} // namespace dt

#endif
