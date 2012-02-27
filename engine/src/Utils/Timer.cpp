
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

Timer::Timer(const QString& message, double interval, bool repeat, bool threaded)
    : mMessage(message),
      mInterval(interval),
      mRepeat(repeat),
      mThreaded(threaded) {
    // start the timer
    if(threaded) {
        _RunThread();
    } else {
        mTimeLeft = mInterval;
        QObject::connect(Root::GetInstance().GetStateManager(), SIGNAL(BeginFrame(double)), 
                this, SLOT(UpdateTimeLeft(double)));
    }
}

void Timer::TriggerTickEvent() {
    emit TimerTicked(mMessage, mInterval);

    if(mRepeat) {
        if(mThreaded) {
            while(mRepeat) {
                sf::Sleep(sf::Seconds(GetInterval()));
                emit TimerTicked(mMessage, mInterval);
            }
        } else {
            mTimeLeft = mInterval;
        }
    } else {
        Stop();
    }
}

double Timer::GetInterval() const {
    return mInterval;
}

const QString& Timer::GetMessageEvent() const {
    return mMessage;
}

void Timer::_RunThread() {
    mThread = std::shared_ptr<sf::Thread>(new sf::Thread(&Timer::_ThreadFunction, this));
    mThread->Launch();
}

void Timer::_ThreadFunction(void* user_data) {
    Timer* timer = (Timer*)user_data;

    // wait for interval, convert to milliseconds for SFML
    sf::Sleep(sf::Seconds(timer->GetInterval()));
    
    // done, trigger event
    timer->TriggerTickEvent();
}

void Timer::TriggerTick() {
    emit TimerTicked("DEBUG", mInterval);
}

void Timer::UpdateTimeLeft(const double& frame_time) {
    mTimeLeft -= frame_time;
    if(mTimeLeft <= 0) {
        TriggerTickEvent();
    }       
}

void Timer::Stop() {
    if(mThreaded) {
        mThread->Terminate();
    } else {
        disconnect(Root::GetInstance().GetStateManager(), SIGNAL(BeginFrame(double)), 
                   this, SLOT(UpdateTimeLeft(double)));
        mTimeLeft = mInterval; // reset
    }
    emit TimerStoped();
}

} // namespace dt
