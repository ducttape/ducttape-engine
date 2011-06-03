#ifndef DUCTTAPE_ENGINE_EVENT_EVENTMANAGER
#define DUCTTAPE_ENGINE_EVENT_EVENTMANAGER

#include <vector>

#include "Event.hpp"
#include "EventListener.hpp"

namespace dt {

/**
  * Class holding all EventListeners, which is responsible to send all Events through the
  * Event system.
  * @see EventListener
  * @see Event
  */
class EventManager {
public:
    /**
      * Default constructor.
      */
    EventManager();

    /**
      * Method used to initiate an Event being sent to all EventListeners.
      * @param event The Event to be sent.
      */
    void HandleEvent(Event* event);

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
private:
    std::vector<EventListener*> mListeners; //!< The list of EventListeners.
};

}

#endif
