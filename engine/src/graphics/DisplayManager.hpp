#ifndef DUCTTAPE_ENGINE_GRAPHICS_DISPLAYMANAGER
#define DUCTTAPE_ENGINE_GRAPHICS_DISPLAYMANAGER

#include <memory>
#include <string>

#include <OgreRoot.h>
#include <OgreRenderSystem.h>
#include <OgreRenderWindow.h>
#include <OgreViewport.h>
#include <OgreWindowEventUtilities.h>

#include "component/CameraComponent.hpp"
#include "utils/Logger.hpp"

namespace dt {

/**
  * A manager class for managing the display and rendering.
  */
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
      * This will fail if the same CameraComponent is already registered. The first CameraComponent that gets registered will also trigger the creation of the render window.
      * @param camera_component Pointer to to the camera component. It should come out of the
      * ComponentsManager to ensure the pointer will always be safe and sound.
      * @returns \c true for success and \c false in case the CameraComponent is already registered.
      */
    bool RegisterCamera(CameraComponent* camera_component);

    /**
      * Tries to unregister a camera.
      * This will fail if the CameraComponent is not registered. Unregistering the last CameraComponent will trigger the destruction of the render window.
      * @param camera_component Pointer to to the camera component. It should come out of the
      * ComponentsManager to ensure the pointer will always be safe and sound.
      * @returns \c true for success and \c false in case the CameraComponent is not found.
      */
    bool UnregisterCamera(CameraComponent* camera_component);

    /**
      * Activates a camera for the active viewport. 
      * This will fail if this CameraComponent has not yet been registered.
      * @param name The name of the CameraComponent.
      * @returns \c true for success and \c false in case the CameraComponent is unknown.
      */
    bool ActivateCamera(const std::string& name);

    /**
      * Renders the current frame.
      */
    void Render();

    Ogre::SceneManager* GetSceneManager(const std::string& scene);

    void CreateOgreRoot();

    Ogre::RenderWindow* GetRenderWindow();

private:
    /**
      * Creates the render window and sets up Ogre. It is called when the first CameraComponent is registered. 
      */
    void _CreateWindow();

    /**
      * Destroy the render window and kills Ogre. It is called after the last CameraComponent has been unregistered. 
      */
    void _DestroyWindow();

    std::map<std::string, CameraComponent*> mCameras;
    std::map<std::string, Ogre::SceneManager*> mSceneManagers;

    Ogre::Root* mOgreRoot;
    Ogre::RenderSystem* mOgreRenderSystem;
    Ogre::RenderWindow* mOgreRenderWindow;
};

}

#endif
