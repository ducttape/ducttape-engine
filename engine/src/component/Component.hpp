#ifndef DUCTTAPE_ENGINE_COMPONENT_COMPONENT
#define DUCTTAPE_ENGINE_COMPONENT_COMPONENT

#include "event/EventListener.hpp"
#include "ComponentListener.hpp"


#include <string>
#include <type_traits>

namespace dt {

// forward declaration
class Node;

/**
  * Modifier for a node. This will add all the functionality to an otherwise empty node,
  * such as a mesh or sound.
  */
class Component : public EventListener {
public:
    // static_assert(std::is_base_of<ComponentListener, ComponentListener>::value, "Cannot create Component with ComponentListener not being inherited from ComponentListener.");

    /**
      * Constructor with set name.
      * @param name The Component name.
      * @param custom_listener The ComponentListener to use for callbacks.
      */
    Component(const std::string& name = "", ComponentListener* custom_listener = nullptr) {
        if(name == "")
            mName = "component-generated-name"; // TODO
        else
            mName = name;
        mListener = std::shared_ptr<ComponentListener>(custom_listener);
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

    /**
      * Returns the ComponentListener of this Component.
      * @returns The ComponentListener of this Component.
      */
    std::shared_ptr<ComponentListener> GetListener() {
        return mListener;
    }

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

protected:
    std::string mName;                          //!< The Component name.
    std::shared_ptr<ComponentListener> mListener;    //!< The ComponentListener to use for callbacks.
    Node* mNode;

private:
    bool mIsActivated;
};

}

#endif
