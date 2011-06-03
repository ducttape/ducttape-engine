#ifndef DUCTTAPE_ENGINE_COMPONENT_TRIGGERCOMPONENT
#define DUCTTAPE_ENGINE_COMPONENT_TRIGGERCOMPONENT

#include "Component.hpp"
#include "TriggerComponentListener.hpp"


namespace dt {

class TriggerComponent : public Component<TriggerComponentListener> {
public:
    TriggerComponent(TriggerComponentListener* custom_listener = new TriggerComponentListener());
    TriggerComponent(const std::string& name, TriggerComponentListener* custom_listener = new TriggerComponentListener());

    virtual void Update(float time_delta);
    virtual void HandleEvent(Event* e);
};

}

#endif
