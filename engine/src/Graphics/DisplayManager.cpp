
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
      mOgreRenderParams(nullptr),
      mNextZOrder(0),
      mWindowSize(Ogre::Vector2(1024, 768)),
      mFullscreen(false) {}

DisplayManager::~DisplayManager() {}

void DisplayManager::initialize() {}

void DisplayManager::deinitialize() {
    if(mOgreRoot != nullptr) {
        _destroyWindow();
    }
}

DisplayManager* DisplayManager::get() {
    return Root::getInstance().getDisplayManager();
}

void DisplayManager::setRenderWindowParams(Ogre::NameValuePairList* params) {
    mOgreRenderParams = params;
}

void DisplayManager::setWindowSize(int width, int height) {
    mWindowSize.x = width;
    mWindowSize.y = height;

    if(mOgreRoot != nullptr) {
        mOgreRenderWindow->resize(width, height);
    }
}

void DisplayManager::setFullscreen(bool fullscreen, bool adjust_resolution) {
    mFullscreen = fullscreen;

    if(mOgreRoot != nullptr) {
        if(adjust_resolution) {
            sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
            mOgreRenderWindow->setFullscreen(fullscreen, desktop.width, desktop.height);
            // do not save as mWindowSize, so when we toggle back, we will get the old resolution back
        } else {
            mOgreRenderWindow->setFullscreen(fullscreen, mWindowSize.x, mWindowSize.y);
        }
    }
}

void DisplayManager::render() {
    if(mOgreRoot != nullptr && mOgreRoot->isInitialised()) {
        mOgreRoot->renderOneFrame();
        Ogre::WindowEventUtilities::messagePump();
    }
}

Ogre::SceneManager* DisplayManager::getSceneManager(const QString scene) {
    if(mSceneManagers.count(scene) == 0) {
        _createWindow(); // TODO check if window already present

        Logger::get().info("Creating a scene manager for scene " + scene + ".");
        Ogre::SceneManager* mgr = mOgreRoot->createSceneManager("DefaultSceneManager");
        mgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));
        mgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_MODULATIVE);
        mSceneManagers[scene] = mgr;
    }
    return mSceneManagers[scene];
}

Ogre::RenderWindow* DisplayManager::getRenderWindow() {
    return mOgreRenderWindow;
}

CameraComponent* DisplayManager::getMainCamera() {
    return mMainCamera;
}

void DisplayManager::setMainCamera(CameraComponent* camera_component) {
    mMainCamera = camera_component;
}

void DisplayManager::_createWindow() {
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
    // Ogre prints an error because it can't find an icon, "GLX_icon.png".
    // This resource is added in order to avoid the error message.
    Ogre::ResourceGroupManager::getSingletonPtr()->addResourceLocation("../data", "FileSystem", "General");
    mOgreRenderWindow = mOgreRoot->createRenderWindow("RenderWindow",
                                                      static_cast<int>(mWindowSize.x),
                                                      static_cast<int>(mWindowSize.y),
                                                      false,
                                                      mOgreRenderParams);

    // Attach OIS
    InputManager::get()->setWindow(mOgreRenderWindow);
    InputManager::get()->initialize();
}

void DisplayManager::_destroyWindow() {
    // Make sure to destroy the GUI first (it won't do anything if it was not initialized)
    mGuiManager.deinitialize();

    // Unattach OIS before window shutdown (very important under Linux)
    Root::getInstance().getInputManager()->deinitialize();

    mOgreRenderWindow->destroy();
    mOgreRoot->shutdown();
    delete mOgreRoot;
}

void DisplayManager::createOgreRoot() {
    if(mOgreRoot == nullptr) {
        _createWindow();
    }
}

GuiManager* DisplayManager::getGuiManager() {
    return &mGuiManager;
}

int DisplayManager::getNextZOrder() {
    mNextZOrder++;
    return mNextZOrder;
}

}

