#ifndef DUCTTAPE_ENGINE_COMPONENT_TRIGGERCOMPONENT
#define DUCTTAPE_ENGINE_COMPONENT_TRIGGERCOMPONENT

#include "Component.hpp"
#include "TriggerComponentListener.hpp"


namespace dt {

/**
  * A simple trigger component that triggers a callback on certain conditions.
  * @warning Class not complete.
  */
class TriggerComponent : public Component<TriggerComponentListener> {
public:
    /**
      * Advanced constructor.
      * @see Component
      */
    TriggerComponent(TriggerComponentListener* custom_listener = new TriggerComponentListener());

    /**
      * Advanced constructor.
      * @see Component
      */
    TriggerComponent(const std::string& name, TriggerComponentListener* custom_listener = new TriggerComponentListener());

    virtual void HandleEvent(Event* e);
};

}

#endif
