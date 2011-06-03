#include "EventManager.hpp"

namespace dt {

EventManager::EventManager() {}

void EventManager::HandleEvent(Event* event) {
    for(EventListener* l: mListeners) {
        l->HandleEvent(event);
    }
}

bool EventManager::HasListener(EventListener* listener) {
    for(EventListener* l: mListeners) {
        if(l == listener)
            return true;
    }
    return false;
}

void EventManager::AddListener(EventListener* listener) {
    if(!HasListener(listener))
        mListeners.push_back(listener);
}

void EventManager::RemoveListener(EventListener* listener) {
    std::vector<EventListener*> new_listeners;

    for(auto iter = mListeners.begin(); iter != mListeners.end(); ++iter) {
        if(*iter != listener)
            new_listeners.push_back(*iter);
    }

    mListeners = new_listeners;
}

}
