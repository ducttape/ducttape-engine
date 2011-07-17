
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_COMPONENT_CAMERACOMPONENT 
#define DUCTTAPE_ENGINE_COMPONENT_CAMERACOMPONENT

#include <string>

#include <OGRE/OgreCamera.h>
#include <OGRE/OgreViewport.h>

#include "component/Component.hpp"

namespace dt {

/**
  * Adds a camera to the Node.
  */
class CameraComponent : public Component {
public:
    /**
      * Advanced constructor.
      * @see Component
      */
    CameraComponent(const std::string& name);

    virtual void HandleEvent(Event* e);

    void OnCreate();
    void OnDestroy();
    void OnUpdate(float time_diff);

    /**
      * Rotates the camera to look at a target point.
      * @param target_point The point to look at.
      */
    void LookAt(Ogre::Vector3 target_point);

private:
    Ogre::Camera* mCamera;      //!< The Ogre camera instance.
    Ogre::Viewport* mViewport;  //!< The viewport of the camera.
};

}

#endif
