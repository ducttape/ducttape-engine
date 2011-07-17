
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

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
#include "graphics/Viewport.hpp"

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
      * It will also run AddViewport with name \c main and set it as main Viewport.
      * @param camera_component Pointer to to the camera component. It should come out of the
      * Node itself to ensure the pointer will always be safe and sound.
      * @returns \c true for success and \c false in case the CameraComponent is already registered.
      */
    bool RegisterCamera(CameraComponent* camera_component);

    /**
      * Tries to unregister a camera.
      * This will fail if the CameraComponent is not registered. Unregistering the last CameraComponent will trigger the destruction of the render window.
      * @param camera_component Pointer to to the camera component. It should come out of the
      * Node itself to ensure the pointer will always be safe and sound.
      * @returns \c true for success and \c false in case the CameraComponent is not found.
      */
    bool UnregisterCamera(CameraComponent* camera_component);

    /**
      * Activates a camera for the active viewport. 
      * This will fail if this CameraComponent has not yet been registered.
      * @param name The name of the CameraComponent.
      * @returns \c true for success and \c false in case the CameraComponent is unknown.
      */
    bool ActivateCamera(const std::string& name, const std::string& viewport_name = "");

    /**
      * Adds Viewport to current window.
      * @param name The name of Viewport.
      * @param camera_name Name of the camera which should be initiated with Viewport.
      * @param set_as_main Sets as main currently created viewport. It will be taken into consideration when doing any sound calculations. It will be also default viewport to assign camera to.
      * @param left From 0.0 to 1.0, tells where to put viewport from left edge of screen.
      * @param top From 0.0 to 1.0, tells where to put viewport from top edge of screen.
      * @param width From 0.0 to 1.0, tells how much width of screen should viewport occupy.
      * @param height From 0.0 to 1.0, tells how much height of screen should viewport occupy.
      */
    bool AddViewport(const std::string& name, const std::string& camera_name, 
                     bool set_as_main = false, float left = 0.0F, float top = 0.0F, 
                     float width = 1.0F, float height = 1.0F);

    /**
      * Hides Viewport from visibility on screen. Remember to not hide viewport that has no viewport under it.
      * @param name Name of Viewport to hide.
      */
    bool HideViewport(const std::string& name);

    /**
      * Shows previously hidden Viewport.
      * @param name Name of Viewport to show.
      */
    bool ShowViewport(const std::string& name);

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
    boost::ptr_map<std::string, dt::Viewport*> mViewports;
    std::map<std::string, std::string> mViewportsCameras;
    std::string mMainViewport;

    Ogre::Root* mOgreRoot;
    Ogre::RenderSystem* mOgreRenderSystem;
    Ogre::RenderWindow* mOgreRenderWindow;
    
    int mNextZOrder;
};

}

#endif
