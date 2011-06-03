#ifndef DUCTTAPE_ENGINE_EVENT_TIMER
#define DUCTTAPE_ENGINE_EVENT_TIMER

#include <memory>

#include <SFML/System/Thread.hpp>

#include "TimerTickEvent.hpp"
#include "BeginFrameEvent.hpp"
#include "EventListener.hpp"

namespace dt {

class Timer : public EventListener {
public:
    Timer(const std::string& message, uint32_t interval, bool repeat = true, bool threaded = false);
    void HandleEvent(Event* e);

    void TriggerTickEvent();

    uint32_t GetInterval() const;
    const std::string& GetMessage() const;
private:
    void _RunThread();
    static void _ThreadFunction(void* user_data);

    std::shared_ptr<sf::Thread> mThread;
    std::string mMessage;
    uint32_t mInterval;
    bool mRepeat;
    bool mThreaded;

    uint32_t mTimeLeft;
};

}

#endif
