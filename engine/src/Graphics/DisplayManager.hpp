
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
      * Sets a camera component as main camera.
      * @param camera_component The new main camera.
      */
    void SetMainCamera(CameraComponent* camera_component);

    /**
      * Returns the GuiManager.
      * @returns The GuiManager.
      */
    GuiManager* GetGuiManager();

    /**
      * Returns the z-order for the next viewport that is being created.
      * @return A new z-order index.
      */
    int GetNextZOrder();

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

    std::map<QString, Ogre::SceneManager*> mSceneManagers; //!< The list of scene managers for the scenes.
    CameraComponent* mMainCamera; //!< Pointer to the main camera component (usually the first / only one).

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
