
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_EVENT_EVENTLISTENER
#define DUCTTAPE_ENGINE_EVENT_EVENTLISTENER

#include <Config.hpp>

#include <Event/Event.hpp>

#include <cstdint>
#include <memory>

namespace dt {

/**
  * Interface for Event listeners.
  * @see Event
  * @see EventManager
  */
class DUCTTAPE_API EventListener {
public:
    /**
      * The priority of a listener. Influences the calling order.
      */
    enum Priority : uint8_t {
        MONITOR = 1,            //!< (1) Only monitor events here, do not cancel the event here.
        INTERNAL_HIGHEST = 2,   //!< (2) Only for internal usage. It is possible but highly discouraged to use this outside of the engine.
        HIGHEST = 3,            //!< (3) Highest priority.
        HIGH = 4,               //!< (4) High priority.
        NORMAL = 5,             //!< (5) Normal priority. (Default)
        LOW = 6,                //!< (6) Low priority.
        LOWEST = 7              //!< (7) Lowest priority.
    };

    /**
      * Pure virtual destructor to make this class abstract.
      */
    virtual ~EventListener() = 0;

    /**
      * Returns the priority of the listener. Override this to set a non-default priority. (Default: NORMAL).
      * @warning This only affects the event flow when a new Listener is being added. If you want to change the Priority during
      * runtime, you need to call EventManager::UpdatePriorities() after you have changed the outcome of this method.
      * @see EventListener::Priority
      * @returns The priority of the listener.
      */
    virtual Priority GetEventPriority() const;

    /**
      * Required to sort the EventListeners by priority in the EventManager.
      * @param l The first EventListener
      * @param r The second EventListener
      * @returns Whether the first listener has higher Priority than the second listener.
      * @see std::sort
      */
    static bool SortHelper(const EventListener* l, const EventListener* r);

};

}

#endif
