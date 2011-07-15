#include "graphics/DisplayManager.hpp"

namespace dt {

DisplayManager::DisplayManager() {
    mOgreRoot = nullptr;
    mNextZOrder = 1;
    mMainViewport = "";
}

DisplayManager::~DisplayManager() {}

void DisplayManager::Initialize() {}

void DisplayManager::Deinitialize() {
    if(mOgreRoot != nullptr) {
        _DestroyWindow();
    }
}

bool DisplayManager::RegisterCamera(CameraComponent* camera_component) {
    std::string name = camera_component->GetName();

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
    std::string name = camera_component->GetName();

    // Do not remove if the requested CameraComponent hasn't been registered.
    if(mCameras.count(name) == 0)
        return false;

    mCameras.erase(name);

    // Destroy the render window if this is the last CameraComponent.
    if(mCameras.size() == 0 && (mOgreRoot != nullptr && mOgreRoot->isInitialised()))
        _DestroyWindow();

    return true;
}

bool DisplayManager::ActivateCamera(const std::string& name, const std::string& viewport_name) {
	// Do not change if the requested CameraComponent hasn't been registered.
    if(mCameras.count(name) == 0)
        return false;

    std::string change_viewport_name;

    if(viewport_name == "") {
        if(mMainViewport == "") {
            if(AddViewport("main", name, true))
            {
                change_viewport_name = mMainViewport;
            } else {
                return false;
            }
        } else {
            change_viewport_name = mMainViewport;
        }
    } else {
        if(mViewports.count(viewport_name) == 0)
            return false;

        change_viewport_name = viewport_name;
    }

    // Do not change if the requested Viewport hasn't been created.
    if(mViewports.count(change_viewport_name) == 0)
        return false;

    mViewports[change_viewport_name]->setCamera(mCameras[name]->GetCamera());
    mViewportsCameras[change_viewport_name] = name;
    mOgreRenderWindow->_updateViewport(mViewports[change_viewport_name]);

    //mOgreViewport->setCamera(mCameras[name]->GetCamera());

	//mOgreRenderWindow->_updateViewport(mOgreViewport);

    return true;
}

bool DisplayManager::AddViewport(const std::string& name, const std::string& camera_name, bool set_as_main, float left, float top, float width, float height)
{
    // Do not add if a Viewport of the same name already exists.
    if(mViewports.count(name) != 0)
        return false;
    // Cannot assign if there is no such camera.
    if(mCameras.count(camera_name) == 0)
        return false;

    mViewports[name] = GetRenderWindow()->addViewport(mCameras[camera_name]->GetCamera(), mNextZOrder, left, top, width, height);
    mNextZOrder++;

    if(set_as_main) {
        mMainViewport = name;
    }

    mViewports[name]->setBackgroundColour(Ogre::ColourValue(0, 0, 0));

    ActivateCamera(camera_name, name);

    return true;
}

void DisplayManager::Render() {
    if(mOgreRoot != nullptr && mOgreRoot->isInitialised()) {
        mOgreRoot->renderOneFrame();
        Ogre::WindowEventUtilities::messagePump();
    }
}

Ogre::SceneManager* DisplayManager::GetSceneManager(const std::string& scene) {
    if(mSceneManagers.count(scene) == 0) {
        _CreateWindow(); // TODO check if window already present

        Logger::Get().Info("Creating a scene manager for scene " + scene + ".");
        Ogre::SceneManager* mgr = mOgreRoot->createSceneManager("DefaultSceneManager");
        mgr->setAmbientLight(Ogre::ColourValue(0.5,0.5,0.5));
        mSceneManagers[scene] = mgr;
    }
    return mSceneManagers[scene];
}

Ogre::RenderWindow* DisplayManager::GetRenderWindow() {
    return mOgreRenderWindow;
}

void DisplayManager::_CreateWindow() {
    if(mOgreRoot != nullptr) {
        return;
    }
    mOgreRoot = new Ogre::Root();

#ifdef COMPILER_MSVC
    mOgreRoot->loadPlugin("RenderSystem_GL.dll");
#else
    mOgreRoot->loadPlugin("/usr/lib/OGRE/RenderSystem_GL.so");
#endif
    mOgreRenderSystem = mOgreRoot->getRenderSystemByName("OpenGL Rendering Subsystem");
    mOgreRoot->setRenderSystem(mOgreRenderSystem);

    mOgreRoot->initialise(false, "Ducttape Game Engine");
    mOgreRenderWindow = mOgreRoot->createRenderWindow("RenderWindow", 1024, 768, false);
}

void DisplayManager::_DestroyWindow() {
    mOgreRenderWindow->destroy();
    mOgreRoot->shutdown();
    delete mOgreRoot;
}

void DisplayManager::CreateOgreRoot() {
    if(mOgreRoot == nullptr) {
        _CreateWindow();
    }
}

}

