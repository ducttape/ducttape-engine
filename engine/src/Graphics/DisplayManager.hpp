
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_GRAPHICS_DISPLAYMANAGER
#define DUCTTAPE_ENGINE_GRAPHICS_DISPLAYMANAGER

#include <Config.hpp>

#include <Core/Manager.hpp>
#include <Graphics/CameraComponent.hpp>
#include <Graphics/Viewport.hpp>
#include <Gui/GuiManager.hpp>

#include <boost/ptr_container/ptr_map.hpp>

#include <OgreRenderSystem.h>
#include <OgreRenderWindow.h>
#include <OgreSceneManager.h>
#include <OgreRoot.h>

#include <QString>

#include <map>

namespace dt {

/**
  * A manager class for managing the display and rendering.
  */
class DUCTTAPE_API DisplayManager : public Manager {
    Q_OBJECT
public:
    /**
      * Default constructor.
      */
    DisplayManager();

    /**
      * Default destructor.
      */
    ~DisplayManager();

    void Initialize();
    void Deinitialize();

    /**
      * Returns a pointer to the Manager instance.
      * @returns A pointer to the Manager instance.
      */
    static DisplayManager* Get();

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
      * @param viewport_name The name of the Viewport.
      * @returns \c true for success and \c false in case the CameraComponent is unknown.
      */
    bool ActivateCamera(const QString& name, const QString& viewport_name = "");

    /**
      * Adds Viewport to current window.
      * @param name The name of Viewport.
      * @param camera_name Name of the camera which should be initiated with Viewport.
      * @param set_as_main Sets as main currently created viewport. It will be taken into consideration when doing any sound calculations. It will be also default viewport to assign camera to.
      * @param left From 0.0 to 1.0, tells where to put viewport from left edge of screen.
      * @param top From 0.0 to 1.0, tells where to put viewport from top edge of screen.
      * @param width From 0.0 to 1.0, tells how much width of screen should viewport occupy.
      * @param height From 0.0 to 1.0, tells how much height of screen should viewport occupy.
      * @returns Whether the operation was successful or not.
      */
    bool AddViewport(const QString& name, const QString& camera_name,
                     bool set_as_main = false, float left = 0.0F, float top = 0.0F,
                     float width = 1.0F, float height = 1.0F);

    /**
      * Hides Viewport from visibility on screen. Remember to not hide viewport that has no viewport under it.
      * @param name Name of Viewport to hide.
      */
    void HideViewport(const QString& name);

    /**
      * Shows previously hidden Viewport.
      * @param name Name of Viewport to show.
      */
    void ShowViewport(const QString& name);

    /**
      * Renders the current frame.
      */
    void Render();

    /**
      * Returns the Ogre::SceneManager for a scene. Creates a new SceneManager if none exists for that scene.
      * @param scene The name of the scene.
      * @returns A pointer to the Ogre::SceneManager.
      */
    Ogre::SceneManager* GetSceneManager(const QString& scene);

    /**
      * Initializes the Ogre Render System.
      */
    void CreateOgreRoot();

    /**
      * Returns the RenderWindow.
      * @returns The window.
      */
    Ogre::RenderWindow* GetRenderWindow();

    /**
      * Returns the main CameraComponent.
      * @returns The main CameraComponent.
      */
    CameraComponent* GetMainCamera();

    /**
      * Returns the GuiManager.
      * @returns The GuiManager.
      */
    GuiManager* GetGuiManager();

public slots:
    /**
      * Sets the window size.
      * @param width The window width, in pixels.
      * @param height The window height, in pixels.
      */
    void SetWindowSize(int width, int height);

    /**
      * Sets if the window is displayed fullscreen.
      * @param fullscreen Whether it should be fullscreen.
      * @param adjust_resolution If true, the window's resolution will be adjusted to match the current desktop resolution.
      */
    void SetFullscreen(bool fullscreen, bool adjust_resolution = true);

private:
    /**
      * Creates the render window and sets up Ogre. It is called when the first CameraComponent is registered.
      */
    void _CreateWindow();

    /**
      * Destroy the render window and kills Ogre. It is called after the last CameraComponent has been unregistered.
      */
    void _DestroyWindow();

    std::map<QString, CameraComponent*> mCameras;               //!< The list of camera components.
    std::map<QString, Ogre::SceneManager*> mSceneManagers;      //!< The list of scene manager for the scenes.
    boost::ptr_map<QString, dt::Viewport> mViewports;           //!< The list of viewports.
    std::map<QString, QString> mViewportsCameras;           //!< The assignment map for cameras<>viewports.
    QString mMainViewport;  //!< The name of the main viewport.
    QString mMainCamera;    //!< The name of the main camera.

    Ogre::Root* mOgreRoot;      //!< The Ogre::Root instance.
    Ogre::RenderSystem* mOgreRenderSystem;  //!< The Ogre::RenderSystem instance.
    Ogre::RenderWindow* mOgreRenderWindow;  //!< The render window.

    GuiManager mGuiManager;     //!< The GuiManager.

    int mNextZOrder;            //!< The z-order for the next viewport to be created.
    Ogre::Vector2 mWindowSize;  //!< The size of the window, in pixels (integer).
    bool mFullscreen;           //!< Whether the window should be fullscreen.
};

}

#endif
