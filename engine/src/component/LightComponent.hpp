#ifndef DUCTTAPE_ENGINE_COMPONENT_LIGHTCOMPONENT 
#define DUCTTAPE_ENGINE_COMPONENT_LIGHTCOMPONENT

#include <string>

#include <OgreLight.h>

#include "component/Component.hpp"

namespace dt {

/**
  * Adds a light to the scene.
  */
class LightComponent : public Component {
public:
    /**
      * Advanced constructor.
      * @see Component
      * @param name The name of the component.
      */
    LightComponent(const std::string& name);

    virtual void HandleEvent(Event* e);

    void OnActivate();
    void OnDeactivate();
    void OnUpdate(float time_diff);

private:
    Ogre::Light* mLight;    //!< The Ogre::Light instance.
};

}

#endif
