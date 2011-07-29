
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include "graphics/DisplayManager.hpp"

#include "Root.hpp"

namespace dt {

DisplayManager::DisplayManager() {
    mOgreRoot = nullptr;
    mNextZOrder = 1;
    mMainViewport = "";
    mMainCamera = "";
}

DisplayManager::~DisplayManager() {}

void DisplayManager::Initialize() {}

void DisplayManager::Deinitialize() {
    if(mOgreRoot != nullptr) {
        _DestroyWindow();
    }
}

DisplayManager* DisplayManager::Get() {
    return Root::get_mutable_instance().GetDisplayManager();
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

    //if there is no name of Viewport given
    if(viewport_name == "") {
        //if there is no main Viewport made
        if(mMainViewport == "") {
            //if we can create one
            if(AddViewport("main", name, true))
            {
                //set it as Viewport for camera
                change_viewport_name = mMainViewport;
            } else {
                //if we cannot set just return false
                return false;
            }
        //if there is main Viewport set
        } else {
            //just use it to activate camera with it
            change_viewport_name = mMainViewport;
        }
    //if someone gave name of Viewport to use
    } else {
        //then use it
        change_viewport_name = viewport_name;
    }

    // Do not activate if the requested Viewport hasn't been created.
    if(mViewports.count(change_viewport_name) == 0)
        return false;

    mViewports[change_viewport_name].setCamera(mCameras[name]->GetCamera());
    mViewportsCameras[change_viewport_name] = name;

    return true;
}

bool DisplayManager::AddViewport(const std::string& name, const std::string& camera_name, 
                                 bool set_as_main, float left, float top, float width, float height)
{
    // Do not add if a Viewport of the same name already exists.
    if(mViewports.count(name) != 0)
        return false;
    // Cannot assign if there is no such camera.
    if(mCameras.count(camera_name) == 0)
        return false;

	std::string viewport_name = name;

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

    mViewports[name].setBackgroundColour(Ogre::ColourValue(0, 0, 0));

    ActivateCamera(camera_name, name);

    return true;
}

bool DisplayManager::HideViewport(const std::string& name) {
    // Do not hide if a Viewport does not exists.
    if(mViewports.count(name) == 0)
        return false;

    mViewports[name].hide();

    return true;
}

bool DisplayManager::ShowViewport(const std::string& name) {
    // Do not show if a Viewport does not exists.
    if(mViewports.count(name) == 0)
        return false;

    mViewports[name].show();

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
        mgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));
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
    mOgreRenderWindow = mOgreRoot->createRenderWindow("RenderWindow", 1024, 768, false);

    // Attach OIS
    InputManager::Get()->SetWindow(mOgreRenderWindow);
    InputManager::Get()->Initialize();
}

void DisplayManager::_DestroyWindow() {
    // Make sure to destroy the GUI first (it won't do anything if it was not initialized)
    mGuiManager.Deinitialize();

    // Unattach OIS before window shutdown (very important under Linux)
    Root::get_mutable_instance().GetInputManager()->Deinitialize();

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

