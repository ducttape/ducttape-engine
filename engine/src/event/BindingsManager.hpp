
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_EVENT_BINDINGSMANAGER
#define DUCTTAPE_ENGINE_EVENT_BINDINGSMANAGER

#include <boost/ptr_container/ptr_map.hpp>

#include "EventListener.hpp"
#include "EventBinding.hpp"

namespace dt {

/**
  * Holds and controls EventBindings.
  * @see EventBinding
  * @see EventManager
  * @see EventListener
  */
class BindingsManager : public EventListener {
public:
    /**
      * Default constructor.
      */
    BindingsManager();

    /**
      * Destructor.
      */
    ~BindingsManager();

    /**
      * Initializes the manager and registers itself as event listener.
      */
    void Initialize();

    /**
      * Deinitializes the manager.
      */
    void Deinitialize();

    void HandleEvent(std::shared_ptr<Event> e);

    /**
      * Adds a new EventBinding.
      * @param binding The new EventBinding.
      * @returns The ID of the new binding. The ID is required to Unbind it again.
      * @see EventBinding
      * @see void Unbind(uint32_t binding_id);
      */
    uint32_t Bind(EventBinding* binding);

    /**
      * Removes an EventBinding.
      * @param binding_id The ID of the binding.
      */
    void Unbind(uint32_t binding_id);
private:
    /**
      * Returns a new ID for the next binding.
      */
    uint32_t _GetNewId();

    boost::ptr_map<uint32_t, EventBinding> mBindings;   //!< The list of bindings.
};

}

#endif
