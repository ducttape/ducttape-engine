#ifndef DUCTTAPE_ENGINE_COMPONENT_LIGHTCOMPONENT 
#define DUCTTAPE_ENGINE_COMPONENT_LIGHTCOMPONENT

#include <string>

#include <OgreLight.h>

#include "component/Component.hpp"
#include "component/LightComponentListener.hpp"

namespace dt {

class LightComponent : public Component {
public:
    /**
      * Advanced constructor.
      * @see Component
      */
    LightComponent(const std::string& name, LightComponentListener* custom_listener = new LightComponentListener());

    virtual void HandleEvent(Event* e);

    void OnActivate();
    void OnDeactivate();
    void OnUpdate(float time_diff);

private:
    Ogre::Light* mLight;
};

}

#endif
