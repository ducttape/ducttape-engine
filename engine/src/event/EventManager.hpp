#ifndef DUCTTAPE_ENGINE_EVENT_EVENTMANAGER
#define DUCTTAPE_ENGINE_EVENT_EVENTMANAGER

#include <vector>

#include "Event.hpp"
#include "EventListener.hpp"

namespace dt {

class EventManager {
public:
    EventManager();

    void HandleEvent(Event* event);

    void AddListener(EventListener* listener);

    void RemoveListener(EventListener* listener);
private:
    std::vector<EventListener*> mListeners;
};

}

#endif
