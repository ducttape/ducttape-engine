
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include <Graphics/DisplayManager.hpp>

#include <Core/Root.hpp>

#include <SFML/Window/VideoMode.hpp>

namespace dt {

DisplayManager::DisplayManager()
    : mMainViewport(""),
      mMainCamera(""),
      mOgreRoot(nullptr),
      mNextZOrder(1),
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

bool DisplayManager::RegisterCamera(CameraComponent* camera_component) {
    QString name(camera_component->GetName());

    // Do not add if a CameraComponent of the same name already exists.
    if(mCameras.count(name) != 0)
        return false;

    int start_size = mCameras.size();

    // Create the render window if this is the first CameraComponent.
    if(mCameras.size() == 0 && (mOgreRoot == nullptr || !mOgreRoot->isInitialised()))
        _CreateWindow();

    mCameras[name] = camera_component;

    if(start_size == 0) {
        AddViewport("main", name, true);
    }

    return true;
}

bool DisplayManager::UnregisterCamera(CameraComponent* camera_component) {
    QString name = camera_component->GetName();

    // Do not remove if the requested CameraComponent hasn't been registered.
    if(mCameras.count(name) == 0)
        return false;

    mCameras.erase(name);

    // Destroy the render window if this is the last CameraComponent.
    if(mCameras.size() == 0 && (mOgreRoot != nullptr && mOgreRoot->isInitialised()))
        _DestroyWindow();

    return true;
}

bool DisplayManager::ActivateCamera(const QString& name, const QString& viewport_name) {
	// Do not change if the requested CameraComponent hasn't been registered.
    if(mCameras.count(name) == 0)
        return false;

    QString new_viewport_name(viewport_name);

    if(new_viewport_name == "") {
        if(mMainViewport == "") {
            // if there is no main Viewport made
            if(AddViewport("main", name, true)) { // try to create a new main viewport
                new_viewport_name = mMainViewport; //set it as Viewport for camera
            } else {
                Logger::Get().Error("Cannot activate camera " + name + ": No viewport given and no main viewport set.");
                return false;
            }
        } else {
            new_viewport_name = mMainViewport;
        }
    }

    // Do not activate if the requested Viewport hasn't been created.
    if(mViewports.count(new_viewport_name) == 0) {
        Logger::Get().Error("Cannot activate camera " + name + " on viewport " + new_viewport_name + ": Viewport does not exist.");
        return false;
    }

    mViewports[new_viewport_name].SetCamera(mCameras[name]->GetCamera());
    mViewportsCameras[new_viewport_name] = name;

    return true;
}

bool DisplayManager::AddViewport(const QString& name, const QString& camera_name,
                                 bool set_as_main, float left, float top, float width, float height)
{
    // Do not add if a Viewport of the same name already exists.
    if(mViewports.count(name) != 0)
        return false;
    // Cannot assign if there is no such camera.
    if(mCameras.count(camera_name) == 0)
        return false;

    QString viewport_name(name);

    mViewports.insert(viewport_name, new dt::Viewport);
    mViewports[name].Initialize((GetRenderWindow()->addViewport(mCameras[camera_name]->GetCamera(),
                                  mNextZOrder, left, top, width, height)));
    mNextZOrder++;

    if(set_as_main) {
        mMainViewport = name;
        mMainCamera = camera_name;

        // set the scene manager for the gui
        mGuiManager.SetSceneManager(mCameras[mMainCamera]->GetCamera()->getSceneManager());
    }

    mViewports[name].SetBackgroundColor(Ogre::ColourValue(0, 0, 0));

    ActivateCamera(camera_name, name);

    return true;
}

void DisplayManager::HideViewport(const QString& name) {
    // Do not hide if the Viewport does not exist.
    if(mViewports.count(name) == 0) {
        Logger::Get().Warning("Cannot hide viewport \"" + name + "\": viewport does not exist.");
    } else {
        mViewports[name].Hide();
    }
}

void DisplayManager::ShowViewport(const QString& name) {
    // Do not show if the Viewport does not exist.
    if(mViewports.count(name) == 0) {
        Logger::Get().Warning("Cannot show viewport \"" + name + "\": viewport does not exist.");
    } else {
        mViewports[name].Show();
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
        mgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);
        mSceneManagers[scene] = mgr;
    }
    return mSceneManagers[scene];
}

Ogre::RenderWindow* DisplayManager::GetRenderWindow() {
    return mOgreRenderWindow;
}

CameraComponent* DisplayManager::GetMainCamera() {
    if(mCameras.count(mMainCamera) > 0) {
        return mCameras.find(mMainCamera)->second;
    }
    return nullptr;
}


void DisplayManager::_CreateWindow() {
    if(mOgreRoot != nullptr) {
        return;
    }
    mOgreRoot = new Ogre::Root("", "");

    // TODO: These paths have to be determined correctly.
#ifdef COMPILER_MSVC
    mOgreRoot->loadPlugin("RenderSystem_GL.dll");
    mOgreRoot->loadPlugin("Plugin_ParticleFX.dll");
#else
    mOgreRoot->loadPlugin("/usr/lib/OGRE/RenderSystem_GL.so");
    mOgreRoot->loadPlugin("/usr/lib/OGRE/Plugin_ParticleFX.so");
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

}

