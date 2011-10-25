
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include <Graphics/DisplayManager.hpp>

#include <Core/Root.hpp>
#include <Utils/LogManager.hpp>
#include <Input/InputManager.hpp>

#include <SFML/Window/VideoMode.hpp>

namespace dt {

DisplayManager::DisplayManager()
    : mMainCamera(nullptr),
      mOgreRoot(nullptr),
      mNextZOrder(0),
      mWindowSize(Ogre::Vector2(1024, 768)),
      mFullscreen(false) {}

DisplayManager::~DisplayManager() {}

void DisplayManager::Initialize() {}

void DisplayManager::Deinitialize() {
    if(mOgreRoot != nullptr) {
        _DestroyWindow();
    }
}

DisplayManager* DisplayManager::Get() {
    return Root::GetInstance().GetDisplayManager();
}

void DisplayManager::SetWindowSize(int width, int height) {
    mWindowSize.x = width;
    mWindowSize.y = height;

    if(mOgreRoot != nullptr) {
        mOgreRenderWindow->resize(width, height);
    }
}

void DisplayManager::SetFullscreen(bool fullscreen, bool adjust_resolution) {
    mFullscreen = fullscreen;

    if(mOgreRoot != nullptr) {
        if(adjust_resolution) {
            sf::VideoMode desktop = sf::VideoMode::GetDesktopMode();
            mOgreRenderWindow->setFullscreen(fullscreen, desktop.Width, desktop.Height);
            // do not save as mWindowSize, so when we toggle back, we will get the old resolution back
        } else {
            mOgreRenderWindow->setFullscreen(fullscreen, mWindowSize.x, mWindowSize.y);
        }
    }
}

void DisplayManager::Render() {
    if(mOgreRoot != nullptr && mOgreRoot->isInitialised()) {
        mOgreRoot->renderOneFrame();
        Ogre::WindowEventUtilities::messagePump();
    }
}

Ogre::SceneManager* DisplayManager::GetSceneManager(const QString& scene) {
    if(mSceneManagers.count(scene) == 0) {
        _CreateWindow(); // TODO check if window already present

        Logger::Get().Info("Creating a scene manager for scene " + scene + ".");
        Ogre::SceneManager* mgr = mOgreRoot->createSceneManager("DefaultSceneManager");
        mgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));
        mgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_MODULATIVE);
        mSceneManagers[scene] = mgr;
    }
    return mSceneManagers[scene];
}

Ogre::RenderWindow* DisplayManager::GetRenderWindow() {
    return mOgreRenderWindow;
}

CameraComponent* DisplayManager::GetMainCamera() {
    return mMainCamera;
}

void DisplayManager::SetMainCamera(CameraComponent* camera_component) {
    mMainCamera = camera_component;
}

void DisplayManager::_CreateWindow() {
    if(mOgreRoot != nullptr) {
        return;
    }
    mOgreRoot = new Ogre::Root("", "");

    // TODO: These paths have to be determined correctly.
#ifdef DUCTTAPE_SYSTEM_WINDOWS
    mOgreRoot->loadPlugin("RenderSystem_GL.dll");
    mOgreRoot->loadPlugin("Plugin_ParticleFX.dll");
    mOgreRoot->loadPlugin("Plugin_CgProgramManager.dll");
#else
    mOgreRoot->loadPlugin("/usr/lib/OGRE/RenderSystem_GL.so");
    mOgreRoot->loadPlugin("/usr/lib/OGRE/Plugin_ParticleFX.so");
    mOgreRoot->loadPlugin("/usr/lib/OGRE/Plugin_CgProgramManager.so");
#endif
    mOgreRenderSystem = mOgreRoot->getRenderSystemByName("OpenGL Rendering Subsystem");
    mOgreRoot->setRenderSystem(mOgreRenderSystem);

    mOgreRoot->initialise(false, "Ducttape Game Engine");
    mOgreRenderWindow = mOgreRoot->createRenderWindow("RenderWindow", static_cast<int>(mWindowSize.x), static_cast<int>(mWindowSize.y), false);

    // Attach OIS
    InputManager::Get()->SetWindow(mOgreRenderWindow);
    InputManager::Get()->Initialize();
}

void DisplayManager::_DestroyWindow() {
    // Make sure to destroy the GUI first (it won't do anything if it was not initialized)
    mGuiManager.Deinitialize();

    // Unattach OIS before window shutdown (very important under Linux)
    Root::GetInstance().GetInputManager()->Deinitialize();

    mOgreRenderWindow->destroy();
    mOgreRoot->shutdown();
    delete mOgreRoot;
}

void DisplayManager::CreateOgreRoot() {
    if(mOgreRoot == nullptr) {
        _CreateWindow();
    }
}

GuiManager* DisplayManager::GetGuiManager() {
    return &mGuiManager;
}

int DisplayManager::GetNextZOrder() {
    mNextZOrder++;
    return mNextZOrder;
}

}

