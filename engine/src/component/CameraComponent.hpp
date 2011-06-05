#ifndef DUCTTAPE_ENGINE_COMPONENT_CAMERACOMPONENT 
#define DUCTTAPE_ENGINE_COMPONENT_CAMERACOMPONENT

#include <string>

#include <OGRE/OgreCamera.h>
#include <OGRE/OgreViewport.h>

#include "component/Component.hpp"
#include "component/CameraComponentListener.hpp"

namespace dt {

class CameraComponent : public Component {
public:
    /**
      * Advanced constructor.
      * @see Component
      */
    CameraComponent(const std::string& name, CameraComponentListener* custom_listener = new CameraComponentListener());

    virtual void HandleEvent(Event* e);

    void OnActivate();
    void OnDeactivate();
    void OnUpdate();

private:
    Ogre::Camera* mCamera;
    Ogre::Viewport* mViewport;
};

}

#endif
