
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include <Gui/GuiManager.hpp>

#include <Graphics/CameraComponent.hpp>
#include <Graphics/DisplayManager.hpp>
#include <Utils/Logger.hpp>

#include <MyGUI_LogManager.h>

namespace dt {

GuiManager::GuiManager()
    : mGuiSystem(nullptr),
      mPlatform(nullptr),
      mMouseCursorVisible(false),
      mRootGuiWindow("Gui") {}

void GuiManager::Initialize() {
    if(mGuiSystem == nullptr) {
        CameraComponent* c = DisplayManager::Get()->GetMainCamera();
        if(c == nullptr || c->GetCamera() == nullptr) {
            // cannot initialize, there is no camera, so there is no viewport
            Logger::Get().Error("Cannot initialize GUI System - no main camera set.");
            return;
        }
        InputManager* inputMgrPtr = InputManager::Get();
        connect(inputMgrPtr, SIGNAL(sKeyPressed(const OIS::KeyEvent&)), this, SLOT(sKeyPressed(const OIS::KeyEvent&)));
        connect(inputMgrPtr, SIGNAL(sKeyReleased(const OIS::KeyEvent&)), this, SLOT(sKeyReleased(const OIS::KeyEvent&)));
        connect(inputMgrPtr, SIGNAL(sMouseMoved(const OIS::MouseEvent&)), this, SLOT(sMouseMoved(const OIS::MouseEvent&)));
        connect(inputMgrPtr, SIGNAL(sMousePressed(const OIS::MouseEvent&, OIS::MouseButtonID)),
                this, SLOT(sMousePressed(const OIS::MouseEvent&, OIS::MouseButtonID)));
        connect(inputMgrPtr, SIGNAL(sMouseReleased(const OIS::MouseEvent&, OIS::MouseButtonID)),
                this, SLOT(sMouseReleased(const OIS::MouseEvent&, OIS::MouseButtonID)));
        Ogre::SceneManager* scene_mgr = c->GetCamera()->getSceneManager();

        mPlatform = new MyGUI::OgrePlatform();

        // Disable MyGUI output completely, Ogre will throw an exception if a
        // resource cannot be found
        MyGUI::LogManager::getInstance().setSTDOutputEnabled(false);

        mPlatform->initialise(DisplayManager::Get()->GetRenderWindow(), scene_mgr, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

        mGuiSystem = new MyGUI::Gui();
        mGuiSystem->initialise();

//        EventManager::Get()->AddListener(this);

        // Show / hide the mouse cursor.
        SetMouseCursorVisible(mMouseCursorVisible);

        // Create the root widget.
        mRootGuiWindow.Create();
    }
}

void GuiManager::Deinitialize() {
    if(mGuiSystem != nullptr) {
        mGuiSystem->shutdown();
        delete mGuiSystem;
        mGuiSystem = nullptr;

        mPlatform->shutdown();
        delete mPlatform;
        mPlatform = nullptr;
    }
}

void GuiManager::sKeyPressed(const OIS::KeyEvent& event) {
    if (_EventEnabled()) {
        mMyguiInputMgrPtr->injectKeyPress(MyGUI::KeyCode::Enum(event.key), event.text);
    }
}

void GuiManager::sKeyReleased(const OIS::KeyEvent& event) {
    if (_EventEnabled()) {
        mMyguiInputMgrPtr->injectKeyRelease(MyGUI::KeyCode::Enum(event.key));
    }
}

void GuiManager::sMouseMoved(const OIS::MouseEvent& event) {
    if (_EventEnabled()) {
        mMyguiInputMgrPtr->injectMouseMove(event.state.X.abs, event.state.Y.abs, event.state.Z.abs);
    }
}

void GuiManager::sMousePressed(const OIS::MouseEvent& event, OIS::MouseButtonID button) {
    if (_EventEnabled()) {
        mMyguiInputMgrPtr->injectMousePress(event.state.X.abs, event.state.Y.abs, MyGUI::MouseButton::Enum(button));
    }
}

void GuiManager::sMouseReleased(const OIS::MouseEvent& event, OIS::MouseButtonID button) {
    if (_EventEnabled()) {
        mMyguiInputMgrPtr->injectMouseRelease(event.state.X.abs, event.state.Y.abs, MyGUI::MouseButton::Enum(button));
    }
}

void GuiManager::SetSceneManager(Ogre::SceneManager* scene_manager) {
    if(mPlatform != nullptr) {
        mPlatform->getRenderManagerPtr()->setSceneManager(scene_manager);
    }
}

MyGUI::Gui* GuiManager::GetGuiSystem() {
    // initialize if not already happened
    Initialize();
    return mGuiSystem;
}

void GuiManager::SetMouseCursorVisible(bool visible) {
    if(mGuiSystem == nullptr && visible) {
        // we have no gui system, but we want to show the cursor
        // so we need to create the GUI system first
        Initialize();
    }

    if(mGuiSystem != nullptr && mMouseCursorVisible != visible) {
        MyGUI::PointerManager::getInstance().setVisible(visible);
        mMouseCursorVisible = visible;
        emit MouseCursorVisibilityChanged(visible);
    }
}

GuiManager* GuiManager::Get() {
    return DisplayManager::Get()->GetGuiManager();
}

GuiRootWindow& GuiManager::GetRootWindow() {
    return mRootGuiWindow;
}

bool GuiManager::_EventEnabled() {
    auto mygui_inputmgr = MyGUI::InputManager::getInstancePtr();
    if (mGuiSystem == nullptr || mygui_inputmgr == nullptr) {
        return false;
    } else {
        mMyguiInputMgrPtr = mygui_inputmgr;
        return true;
    }
}

}
