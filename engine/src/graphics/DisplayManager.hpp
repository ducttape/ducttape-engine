#ifndef DUCTTAPE_ENGINE_GRAPHICS_DISPLAYMANAGER
#define DUCTTAPE_ENGINE_GRAPHICS_DISPLAYMANAGER

#include <OgreRoot.h>
#include <OgreRenderSystem.h>
#include <OgreRenderWindow.h>
#include <OgreViewport.h>

#include "component/CameraComponent.hpp"

namespace dt {

class DisplayManager {
public:
    /**
      * Default constructor.
      */
    DisplayManager();

    /**
      * Default destructor.
      */
    ~DisplayManager();

    /**
      * Initializer. 
      */
    void Initialize();

    /**
      * Deinitializer.
      */
    void Deinitialize();

    /**
      * Tries to register a camera.
      * This will fail if the same CameraComponent is already registered.
      * @param camera_component Pointer to to the camera component. It should come out of the
      * ComponentsManager to ensure the pointer will always be safe and sound.
      * @returns \ctrue for success and \cfalse in case the CameraComponent is already registered.
      */
    bool RegisterCamera(CameraComponent* camera_component);

    /**
      * Activates a camera for the active viewport. 
      * This will fail if this CameraComponent has not yet been registered.
      * @param The name of the CameraComponent.
      * @returns \ctrue for success and \cfalse in case the CameraComponent is unknown.
      */
    bool ActivateCamera(const std::string& name);

private:
    /**
      * TODO 
      */
    void _CreateWindow();

    /**
      * TODO 
      */
    void _CreateViewport();

    std::map<std::string, CameraComponent*> mCameras;

    Ogre::Root* mOgreRoot;
    Ogre::RenderSystem* mOgreRenderSystem;
    Ogre::RenderWindow* mOgreRenderWindow;
    Ogre::Viewport* mOgreViewport;
};

}

#endif
