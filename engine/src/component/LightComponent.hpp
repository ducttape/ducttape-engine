#ifndef DUCTTAPE_ENGINE_COMPONENT_LIGHTCOMPONENT 
#define DUCTTAPE_ENGINE_COMPONENT_LIGHTCOMPONENT

#include <string>

#include <OgreLight.h>

#include "component/Component.hpp"

namespace dt {

class LightComponent : public Component {
public:
    /**
      * Advanced constructor.
      * @see Component
      */
    LightComponent(const std::string& name);

    virtual void HandleEvent(Event* e);

    void OnActivate();
    void OnDeactivate();
    void OnUpdate(float time_diff);

private:
    Ogre::Light* mLight;
};

}

#endif
