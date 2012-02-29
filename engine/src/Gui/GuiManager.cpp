
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
      mMouseCursorVisible(true),
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
        QObject::connect(inputMgrPtr, SIGNAL(sPressed(dt::InputManager::InputCode, const OIS::EventArg&)),
                                this, SLOT(sPressed(dt::InputManager::InputCode, const OIS::EventArg&)));
        QObject::connect(inputMgrPtr, SIGNAL(sReleased(dt::InputManager::InputCode, const OIS::EventArg&)),
                                this, SLOT(sReleased(dt::InputManager::InputCode, const OIS::EventArg&)));
        QObject::connect(inputMgrPtr, SIGNAL(sMouseMoved(const OIS::MouseEvent&)), this, SLOT(sMouseMoved(const OIS::MouseEvent&)));

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

        // Initialize the root widget.
        mRootGuiWindow.Initialize();
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

void GuiManager::sPressed(dt::InputManager::InputCode input_code, const OIS::EventArg& event) {
    if (_EventEnabled()) {
        if(input_code >= InputManager::MC_LEFT) {
            const OIS::MouseEvent& mouse_event = (const OIS::MouseEvent&)event;

            mMyguiInputMgrPtr->injectMousePress(mouse_event.state.X.abs, mouse_event.state.Y.abs,
                MyGUI::MouseButton::Enum((OIS::MouseButtonID)(input_code - InputManager::MC_LEFT)));
        }
        else {
            const OIS::KeyEvent& key_event = (const OIS::KeyEvent&)event;

            mMyguiInputMgrPtr->injectKeyPress(MyGUI::KeyCode::Enum(key_event.key), key_event.text);
        }
    }
}

void GuiManager::sReleased(dt::InputManager::InputCode input_code, const OIS::EventArg& event) {
    if (_EventEnabled()) {
        if(input_code >= InputManager::MC_LEFT) {
            const OIS::MouseEvent& mouse_event = (const OIS::MouseEvent&)event;

            mMyguiInputMgrPtr->injectMousePress(mouse_event.state.X.abs, mouse_event.state.Y.abs,
                MyGUI::MouseButton::Enum((OIS::MouseButtonID)(input_code - InputManager::MC_LEFT)));
            mMyguiInputMgrPtr->injectMouseRelease(mouse_event.state.X.abs, mouse_event.state.Y.abs,
                MyGUI::MouseButton::Enum((OIS::MouseButtonID)(input_code - InputManager::MC_LEFT)));
        }
        else {
            const OIS::KeyEvent& key_event = (const OIS::KeyEvent&)event;

            mMyguiInputMgrPtr->injectKeyRelease(MyGUI::KeyCode::Enum(key_event.key));
        }
    }
}

void GuiManager::sMouseMoved(const OIS::MouseEvent& event) {
    if (_EventEnabled()) {
        mMyguiInputMgrPtr->injectMouseMove(event.state.X.abs, event.state.Y.abs, event.state.Z.abs);
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
