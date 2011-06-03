#ifndef DUCTTAPE_ENGINE_COMPONENT_COMPONENT
#define DUCTTAPE_ENGINE_COMPONENT_COMPONENT

#include "event/EventListener.hpp"
#include "ComponentListener.hpp"

#include <string>
#include <type_traits>

namespace dt {

/**
  * Modifier for a node. This will add all the functionality to an otherwise empty node,
  * such as a mesh or sound.
  */
template <typename ListenerType>
class Component : public EventListener {
public:
    static_assert(std::is_base_of<ComponentListener, ListenerType>::value, "Cannot create Component with ListenerType not being inherited from ComponentListener.");

    /**
      * Default constructor.
      * @param custom_listener The ComponentListener to use for callbacks.
      */
    Component(ListenerType* custom_listener = new ListenerType()) {
        mName = "component-generated-name";
        mListener = std::shared_ptr<ListenerType>(custom_listener);
    }

    /**
      * Constructor with set name.
      * @param name The Component name.
      * @param custom_listener The ComponentListener to use for callbacks.
      */
    Component(const std::string& name, ListenerType* custom_listener = new ListenerType()) {
        mName = name;
        mListener = std::shared_ptr<ListenerType>(custom_listener);
    }

    /**
      * Pure virtual destructor makes this class abstract.
      */
    virtual ~Component() = 0;

    /**
      * Returns the name of the Component.
      * @returns The name of the Component.
      */
    const std::string& GetName() const {
        return mName;
    }

    /**
      * This is the place where the magic happens.
      * @param time_delta The time passed since last call.
      */
    virtual void Update(float time_delta) {}

    virtual void HandleEvent(Event* e) {}

    /**
      * Returns the ComponentListener of this Component.
      * @returns The ComponentListener of this Component.
      */
    std::shared_ptr<ListenerType> GetListener() {
        return mListener;
    }

private:
    std::string mName;                          //!< The Component name.
    std::shared_ptr<ListenerType> mListener;    //!< The ComponentListener to use for callbacks.

};

}

#endif
