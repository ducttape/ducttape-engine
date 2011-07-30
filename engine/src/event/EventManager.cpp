
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

EventManager::EventManager()
    : mListenersLocked(false) {}

void EventManager::Initialize() {
    mBindingsManager.Initialize();
}

void EventManager::Deinitialize() {
    mBindingsManager.Deinitialize();
}

EventManager* EventManager::Get() {
    return Root::GetInstance().GetEventManager();
}

void EventManager::InjectEvent(std::shared_ptr<Event> event) {
    _LockListeners();
#ifdef COMPILER_MSVC
    BOOST_FOREACH(EventListener* l, mListeners) {
#else
    for(EventListener* l: mListeners) {
#endif
        if(l == nullptr)
            Logger::Get().Error("EventManager: Could not inject Event to listener (nullptr).");
        else {
            l->HandleEvent(event);
            if(event->IsCanceled()) {
                if(l->GetEventPriority() == EventListener::MONITOR) {
                    // events cannot be cancelled in Monitor mode
                    Logger::Get().Warning("EventListener with priority MONITOR cannot cancel events. Use HIGHEST instead. Continuing.");
                    event->Uncancel();
                } else {
                    break;
                }
            }
        }
    }
    _UnlockListeners();
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
    if(mListenersLocked) {
        Logger::Get().Debug("EventManager: Cannot add listener. List locked. Queued listener for adding.");
        mListenerAddQueue.push_back(listener);
        return;
    }

    if(!HasListener(listener)) {
        if(listener == nullptr) {
            Logger::Get().Error("EventManager: Could not add listener. It is NULL.");
        } else {
            mListeners.push_back(listener);
            UpdatePriorities();
        }
    } else {
        Logger::Get().Error("EventManager: Could not add listener - already registered.");
    }
}

void EventManager::RemoveListener(EventListener* listener) {
    if(mListenersLocked) {
        Logger::Get().Debug("EventManager: Cannot remove listener. List locked. Queued listener for removal.");
        mListenerRemoveQueue.push_back(listener);
        return;
    }

    std::vector<EventListener*> new_listeners;

    for(auto iter = mListeners.begin(); iter != mListeners.end(); ++iter) {
        if(*iter != listener)
            new_listeners.push_back(*iter);
    }

    mListeners = new_listeners;
}

void EventManager::UpdatePriorities() {
    std::sort(mListeners.begin(), mListeners.end(), EventListener::SortHelper);
}

BindingsManager* EventManager::GetBindingsManager() {
    return &mBindingsManager;
}


void EventManager::_LockListeners() {
    mListenersLocked = true;
}

void EventManager::_UnlockListeners() {
    mListenersLocked = false;

    while(mListenerRemoveQueue.size() > 0) {
        RemoveListener(mListenerRemoveQueue.back());
        mListenerRemoveQueue.pop_back();
    }

    while(mListenerAddQueue.size() > 0) {
        AddListener(mListenerAddQueue.back());
        mListenerAddQueue.pop_back();
    }
}

}
