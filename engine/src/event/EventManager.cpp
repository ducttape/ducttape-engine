
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifdef COMPILER_MSVC
#include <boost/foreach.hpp>
#endif

#include "Root.hpp"
#include "EventManager.hpp"

namespace dt {

EventManager::EventManager() {}

void EventManager::Initialize() {
    mBindingsManager.Initialize();
}

void EventManager::Deinitialize() {
    mBindingsManager.Deinitialize();
}

EventManager* EventManager::Get() {
    return Root::get_mutable_instance().GetEventManager();
}

void EventManager::InjectEvent(std::shared_ptr<Event> event) {
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
    if(!HasListener(listener)) {
        if(listener == nullptr)
            Logger::Get().Error("EventManager: Could not add listener. It is NULL.");
        else
            mListeners.push_back(listener);
    } else {
        Logger::Get().Error("EventManager: Could not add listener - already registered.");
    }
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
