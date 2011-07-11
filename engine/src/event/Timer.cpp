#include "Timer.hpp"

#include "Root.hpp"

namespace dt {

Timer::Timer(const std::string& message, uint32_t interval, bool repeat, bool threaded) {
    mMessage = message;
    mInterval = interval;
    mThreaded = threaded;
    mRepeat = repeat;

    if(threaded) {
        _RunThread();
    } else {
        Root::get_mutable_instance().GetEventManager()->AddListener(this);
        mTimeLeft = mInterval;
    }
}

void Timer::HandleEvent(Event* e) {
    // for event mode

    if(e->GetType() == "DT_BEGINFRAMEEVENT") {
        // every frame
        BeginFrameEvent* b = (BeginFrameEvent*)e;

        if(mTimeLeft > b->GetFrameTime()) {
            mTimeLeft -= b->GetFrameTime();
        } else {
            // ignore if we have some delay (actually, this is the downside of event mode)
            mTimeLeft = 0;
            TriggerTickEvent();
        }
    }
}

void Timer::TriggerTickEvent() {
    Root::get_mutable_instance().GetEventManager()->HandleEvent(new TimerTickEvent(mMessage, mInterval));

    if(mRepeat && mThreaded) {
        _RunThread();
    }

    if(!mThreaded) {
        if(!mRepeat) {
            // disable
            Root::get_mutable_instance().GetEventManager()->RemoveListener(this);
        } else {
            // reset
            mTimeLeft = mInterval;
        }
    }

}

uint32_t Timer::GetInterval() const {
    return mInterval;
}

const std::string& Timer::GetMessageEvent() const {
    return mMessage;
}

void Timer::_RunThread() {
    mThread = std::shared_ptr<sf::Thread>(new sf::Thread(&Timer::_ThreadFunction, this));
    mThread->Launch();
}

void Timer::_ThreadFunction(void* user_data) {
    Timer* timer = (Timer*)user_data;

    // wait for interval milliseconds
    sf::Sleep(timer->GetInterval());

    // done, trigger event
    timer->TriggerTickEvent();
}


}
