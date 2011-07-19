
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_EVENT_EVENTMANAGER
#define DUCTTAPE_ENGINE_EVENT_EVENTMANAGER

#include <memory>
#include <vector>

#include "Event.hpp"
#include "EventListener.hpp"
#include "BindingsManager.hpp"
#include "Manager.hpp"

namespace dt {

/**
  * Class holding all EventListeners, which is responsible to send all Events through the
  * Event system.
  * @see EventListener
  * @see Event
  */
class EventManager : public Manager {
public:
    /**
      * Default constructor.
      */
    EventManager();

    /**
      * Initializer.
      */
    void Initialize();

    /**
      * Deinitializer.
      */
    void Deinitialize();

    /**
      * Method used to initiate an Event being sent to all EventListeners.
      * This method is basically the entry point for every event being sent.
      * The events are then distributed to all listeners. It should be noted
      * that events are converted into shared_ptrs in this method which is why
      * this method should be used like this:
      * @code Root::get_mutable_instance.GetEventManager()->InjectEvent(new SomeEvent()); @endcode
      * @param event The Event to be sent.
      */
    void InjectEvent(Event* event);

    /**
      * See above. This method should only be used when the event is already
      * present as a shared_ptr, for example when it has been Cloned.
      * @see Event::Clone
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
      * Returns the BindingsManager.
      * @returns The BindingsManager.
      */
    BindingsManager* GetBindingsManager();

private:
    std::vector<EventListener*> mListeners; //!< The list of EventListeners.
    BindingsManager mBindingsManager;
};

}

#endif
