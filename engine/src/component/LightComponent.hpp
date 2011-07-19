
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

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
    LightComponent(const std::string& name = "");

    virtual void HandleEvent(std::shared_ptr<Event> e);

    void OnCreate();
    void OnDestroy();
    void OnUpdate(double time_diff);

private:
    Ogre::Light* mLight;    //!< The Ogre::Light instance.
};

}

#endif
