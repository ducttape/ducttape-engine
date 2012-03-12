
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include <Utils/Timer.hpp>
#include <Scene/StateManager.hpp>
#include <Core/Root.hpp>
#include <iostream>

namespace dt {

Timer::Timer(const QString message, double interval, bool repeat, bool threaded)
    : mMessage(message),
      mInterval(interval),
      mRepeat(repeat),
      mThreaded(threaded) {
    // start the timer
    if(threaded) {
        _runThread();
    } else {
        mTimeLeft = mInterval;
        QObject::connect(Root::getInstance().getStateManager(), SIGNAL(beginFrame(double)),
                         this,                                  SLOT(updateTimeLeft(double)));
    }
}

void Timer::triggerTickEvent() {
    emit timerTicked(mMessage, mInterval);

    if(mRepeat) {
        if(mThreaded) {
            while(mRepeat) {
                sf::sleep(sf::seconds(getInterval()));
                emit timerTicked(mMessage, mInterval);
            }
        } else {
            mTimeLeft = mInterval;
        }
    } else {
        stop();
    }
}

double Timer::getInterval() const {
    return mInterval;
}

const QString Timer::getMessageEvent() const {
    return mMessage;
}

void Timer::_runThread() {
    mThread = std::shared_ptr<sf::Thread>(new sf::Thread(&Timer::_threadFunction, this));
    mThread->launch();
}

void Timer::_threadFunction(void* user_data) {
    Timer* timer = (Timer*)user_data;

    // wait for interval, convert to milliseconds for SFML
    sf::sleep(sf::seconds(timer->getInterval()));
    
    // done, trigger event
    timer->triggerTickEvent();
}

void Timer::triggerTick() {
    emit timerTicked("DEBUG", mInterval);
}

void Timer::updateTimeLeft(const double& frame_time) {
    mTimeLeft -= frame_time;
    if(mTimeLeft <= 0) {
        triggerTickEvent();
    }       
}

void Timer::stop() {
    if(mThreaded) {
        mThread->terminate();
    } else {
        disconnect(Root::getInstance().getStateManager(), SIGNAL(beginFrame(double)),
                   this,                                  SLOT(updateTimeLeft(double)));
        mTimeLeft = mInterval; // reset
    }
    emit timerStoped();
}

} // namespace dt
