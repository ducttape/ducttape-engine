#ifdef COMPILER_MSVC
#include <boost/foreach.hpp>
#endif

#include "EventManager.hpp"

namespace dt {

EventManager::EventManager() {}

void EventManager::HandleEvent(Event* event) {
#ifdef COMPILER_MSVC
    BOOST_FOREACH(EventListener* l, mListeners) {
#else
    for(EventListener* l: mListeners) {
#endif
        l->HandleEvent(event);
    }
}

bool EventManager::HasListener(EventListener* listener) {
#ifdef COMPILER_MSVC
    BOOST_FOREACH(EventListener* l, mListeners) {
#else
    for(EventListener* l: mListeners) {
#endif
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
