#ifndef DUCTTAPE_ENGINE_COMPONENT_TRIGGERCOMPONENTLISTENER
#define DUCTTAPE_ENGINE_COMPONENT_TRIGGERCOMPONENTLISTENER

#include "ComponentListener.hpp"

namespace dt {

/**
  * The ComponentListener for the TriggerComponent.
  * @see TriggerComponent
  */
class TriggerComponentListener : public ComponentListener {
public:
    /**
      * Callback called when TriggerComponent registers a triggering Event.
      */
    void Triggered();


    /**
      * Callback called when TriggerComponent registers a triggering Event.
      */
    virtual void OnTriggered();

};

}

#endif
