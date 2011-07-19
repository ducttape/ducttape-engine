
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifdef COMPILER_MSVC
#include <boost/foreach.hpp>
#endif

#include "EventManager.hpp"

namespace dt {

EventManager::EventManager() {}

void EventManager::Initialize() {
    mBindingsManager.Initialize();
}

void EventManager::Deinitialize() {
    mBindingsManager.Deinitialize();
}

void EventManager::HandleEvent(Event* event) {
    std::shared_ptr<Event> safe_event(event);
#ifdef COMPILER_MSVC
    BOOST_FOREACH(EventListener* l, mListeners) {
#else
    for(EventListener* l: mListeners) {
#endif
        l->HandleEvent(safe_event);
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

BindingsManager* EventManager::GetBindingsManager() {
    return &mBindingsManager;
}

}
