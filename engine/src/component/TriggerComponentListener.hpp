#ifndef DUCTTAPE_ENGINE_COMPONENT_TRIGGERCOMPONENTLISTENER
#define DUCTTAPE_ENGINE_COMPONENT_TRIGGERCOMPONENTLISTENER

#include "ComponentListener.hpp"

namespace dt {

class TriggerComponentListener : public ComponentListener {
public:
    void Triggered();
    virtual void OnTriggered();

};

}

#endif
