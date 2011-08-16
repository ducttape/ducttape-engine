
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_EVENT_EVENTMANAGER
#define DUCTTAPE_ENGINE_EVENT_EVENTMANAGER

#include <Config.hpp>

#include <Core/Manager.hpp>
#include <Event/EventListener.hpp>
#include <Event/BindingsManager.hpp>

#include <memory>
#include <vector>

namespace dt {

/**
  * Class holding all EventListeners, which is responsible to send all Events through the
  * Event system.
  * @see EventListener
  * @see Event
  */
class DUCTTAPE_API EventManager : public Manager {
    Q_OBJECT
public:
    /**
      * Default constructor.
      */
    EventManager();

    void Initialize();
    void Deinitialize();

    /**
      * Returns a pointer to the Manager instance.
      * @returns A pointer to the Manager instance.
      */
    static EventManager* Get();

    /**
      * Method used to initiate an Event being sent to all EventListeners.
      * This method is basically the entry point for every event being sent.
      * The events are then distributed to all listeners. It should be noted
      * that events are converted into shared_ptrs in this method which is why
      * this method should be used like this:
      * @code EventManager::Get()->InjectEvent(std::make_shared<SomeEvent>()); @endcode
      * @param event The Event to be sent.
      */
    void InjectEvent(std::shared_ptr<Event> event);

    /**
      * Checks if the EventListener is in the list.
      * @param listener The EventListener to find.
      * @returns True if the EventListener is registered, otherwise false.
      */
    bool HasListener(EventListener* listener);

    /**
      * Adds an EventListener to the list of listeners.
      * @param listener The EventListener to add.
      */
    void AddListener(EventListener* listener);

    /**
      * Removes an EventListener from the list of listeners.
      * @param listener The EventListener to remove.
      */
    void RemoveListener(EventListener* listener);

    /**
      * Updates the priorities so the listeners will be called in the right order again. This needs to be
      * done when the priority of one EventListener has changed.
      */
    void UpdatePriorities();

    /**
      * Returns the BindingsManager.
      * @returns The BindingsManager.
      */
    BindingsManager* GetBindingsManager();

private:
    /**
      * Locks the list of listener.
      */
    void _LockListeners();

    /**
      * Unlocks the list of listeners and applies all queues. (Queue order: Remove, Add).
      */
    void _UnlockListeners();

    std::vector<EventListener*> mListeners; //!< The list of EventListeners.
    BindingsManager mBindingsManager;       //!< The BindingManager.

    bool mListenersLocked;                              //!< Whether the list of listener is locked (due to being looped over etc.)
    std::vector<EventListener*> mListenerAddQueue;      //!< The queue of listeners to add when the list is unlocked.
    std::vector<EventListener*> mListenerRemoveQueue;   //!< The queue of listeners to remove when the list is unlocked.
};

}

#endif
