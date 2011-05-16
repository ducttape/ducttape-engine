#include "EventManager.hpp"

namespace dt {

EventManager::EventManager() {}

void EventManager::HandleEvent(Event* event) {
    for(EventListener* l: mListeners) {
        l->HandleEvent(event);
    }
}

void EventManager::AddListener(EventListener* listener) {

}

void EventManager::RemoveListener(EventListener* listener) {

}

}
