#ifndef DUCTTAPE_ENGINE_COMPONENT_COMPONENT
#define DUCTTAPE_ENGINE_COMPONENT_COMPONENT

#include "event/EventListener.hpp"

#include <string>

#include <boost/ptr_container/ptr_map.hpp>
#include <boost/signals2.hpp>

namespace dt {

// forward declaration
class Node;

/**
  * Modifier for a node. This will add all the functionality to an otherwise empty node,
  * such as a mesh or sound.
  */
class Component : public EventListener {
public:
    /**
      * Constructor with set name.
      * @param name The Component name.
      */
    Component(const std::string& name = "") {
        if(name == "")
            mName = "component-generated-name"; // TODO
        else
            mName = name;
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

    virtual void HandleEvent(Event* e) {}

    virtual void OnActivate() {}
    virtual void OnDeactivate() {}
    virtual void OnUpdate(float time_diff) {}

    void SetNode(Node* node) {
        mNode = node;
    }

    Node* GetNode() {
        return mNode;
    }

    void Activate();
    void Deactivate();
    bool IsActivated();

    /**
      * Binds a slot to a signal.
      * @param signal_identifier The name of the signal being called on events. Example: "Triggered" or "AnimationFinished". Case sensitive.
      * @param slot The callback function.
      * @returns A boost signals2 connection to disconnect the slot again.
      */
    boost::signals2::connection BindSlot(const std::string& signal_identifier, boost::function<void ()> slot);

protected:

    /**
      * Calls the signal with the identifier given.
      * @param signal_identifier The signal to call.
      */
    void _CallSignal(const std::string& signal_identifier);
    boost::ptr_map<std::string, boost::signals2::signal<void ()> > mSignals;    //!< The list of signals.

    std::string mName;                          //!< The Component name.
    Node* mNode;

private:
    bool mIsActivated;
};

}

#endif
