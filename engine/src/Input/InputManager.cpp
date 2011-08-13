
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include <Input/InputManager.hpp>

#include <Core/Root.hpp>
#include <Utils/Logger.hpp>
#include <Input/MouseEvent.hpp>
#include <Input/KeyboardEvent.hpp>
#include <Graphics/WindowClosedEvent.hpp>
#include <Utils/Utils.hpp>
#include <Event/EventManager.hpp>
#include <Gui/GuiManager.hpp>

namespace dt {

InputManager::InputManager()
    : mWindow(nullptr),
      mInputSystem(nullptr),
      mMouseCursorMode(InputManager::SYSTEM),
      mJailInput(false) {}

void InputManager::Initialize() {
    if(mInputSystem != nullptr) {
        Logger::Get().Warning("Input system already initialized. Aborting.");
        return;
    }

    if(mWindow == nullptr) {
        Logger::Get().Warning("Cannot initialize InputManager: no window set.");
        return;
    }

    _CreateInputSystem();

    // Register as a Window listener
    Ogre::WindowEventUtilities::addWindowEventListener(mWindow, this);
}

void InputManager::Deinitialize() {
    _DestroyInputSystem();
}

InputManager* InputManager::Get() {
    return Root::GetInstance().GetInputManager();
}

void InputManager::SetWindow(Ogre::RenderWindow* window) {
    mWindow = window;
}

void InputManager::Capture() {
    if(mInputSystem != nullptr) {
        // the system has been initialized
        mMouse->capture();
        mKeyboard->capture();
    }
}

void InputManager::SetJailInput(bool jail_input) {
    mJailInput =  jail_input;
}

bool InputManager::GetJailInput() const {
    return mJailInput;
}


bool InputManager::keyPressed(const OIS::KeyEvent& event) {
    EventManager::Get()->
        InjectEvent(std::make_shared<KeyboardEvent>(KeyboardEvent::PRESSED, event.key, event.text));
    return true;
}

bool InputManager::keyReleased(const OIS::KeyEvent& event) {
    EventManager::Get()->
        InjectEvent(std::make_shared<KeyboardEvent>(KeyboardEvent::RELEASED, event.key, event.text));
    return true;
}

bool InputManager::mouseMoved(const OIS::MouseEvent& event) {
    EventManager::Get()->
        InjectEvent(std::make_shared<MouseEvent>(MouseEvent::MOVED, event.state));
    return true;
}

bool InputManager::mousePressed(const OIS::MouseEvent& event, OIS::MouseButtonID button) {
    EventManager::Get()->
        InjectEvent(std::make_shared<MouseEvent>(MouseEvent::PRESSED, event.state, button));
    return true;
}

bool InputManager::mouseReleased(const OIS::MouseEvent& event, OIS::MouseButtonID button) {
    EventManager::Get()->
        InjectEvent(std::make_shared<MouseEvent>(MouseEvent::RELEASED, event.state, button));
    return true;
}

void InputManager::windowResized(Ogre::RenderWindow* window) {
    // update the mouse capture width / height

    unsigned int width, height, depth;
    int left, top;
    window->getMetrics(width, height, depth, left, top);

    mMouse->getMouseState().width = width;
    mMouse->getMouseState().height = height;
}

void InputManager::windowClosed(Ogre::RenderWindow* window) {
    // Only close for window that created OIS
    if(window == mWindow) {
        Logger::Get().Info("The window was closed");
        EventManager::Get()->InjectEvent(std::make_shared<WindowClosedEvent>());
    }
}

OIS::Mouse* InputManager::GetMouse() {
    return mMouse;
}

OIS::Keyboard* InputManager::GetKeyboard() {
    return mKeyboard;
}

void InputManager::SetMouseCursorMode(InputManager::MouseCursorMode mode) {
    if(mMouseCursorMode == mode) {
        // don't do anything
        return;
    }

    GuiManager::Get()->SetMouseCursorVisible(mode == InputManager::GRAPHICAL);

    mMouseCursorMode = mode;

    // restart the input system with the correct settings, so
    // the cursor visibility is being applied
    _ResetInputSystem();
}

InputManager::MouseCursorMode InputManager::GetMouseCursorMode() const {
    return mMouseCursorMode;
}

void InputManager::_ResetInputSystem() {
    _DestroyInputSystem();
    _CreateInputSystem();
}

void InputManager::_DestroyInputSystem() {
    if(mInputSystem != nullptr) {
        Logger::Get().Info("Deinitializing input system");
        mInputSystem->destroyInputObject(mMouse);
        mInputSystem->destroyInputObject(mKeyboard);
        OIS::InputManager::destroyInputSystem(mInputSystem);
        mInputSystem = nullptr;
    }
}

void InputManager::_CreateInputSystem() {
    OIS::ParamList params;

    // getting window handle
    size_t window_handle = 0;
    mWindow->getCustomAttribute("WINDOW", &window_handle);
    params.insert(std::make_pair(std::string("WINDOW"), Utils::ToStdString(Utils::ToString(window_handle))));
    if(!mJailInput) {
// THIS IS TOTALLY BROKEN AND NEEDS FIXING
#if defined OIS_WIN32_PLATFORM
        /*params.insert(std::make_pair(QString("w32_mouse"), QString("DISCL_NONEXCLUSIVE")));
        params.insert(std::make_pair(QString("w32_keyboard"), QString("DISCL_FOREGROUND")));
        params.insert(std::make_pair(QString("w32_keyboard"), QString("DISCL_NONEXCLUSIVE")));*/
#elif defined OIS_LINUX_PLATFORM
        params.insert(std::make_pair(std::string("x11_mouse_grab"), std::string("false")));
        params.insert(std::make_pair(std::string("x11_keyboard_grab"), std::string("false")));
        params.insert(std::make_pair(std::string("XAutoRepeatOn"), std::string("true")));
#endif
    }

#if defined OIS_WIN32_PLATFORM
    // TODO: This stuff crashes OIS, WHY?
    //params.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_FOREGROUND" )));
    //params.insert(std::make_pair(std::string("w32_mouse"), std::string(mMouseCursorMode == InputManager::SYSTEM ? "DISCL_FOREGROUND" : "DISCL_BACKGROUND" )));
#elif defined OIS_LINUX_PLATFORM
    params.insert(std::make_pair(std::string("x11_mouse_hide"), std::string(mMouseCursorMode == InputManager::SYSTEM ? "false" : "true")));
#endif

    Logger::Get().Info("Initializing input system (Window: " + Utils::ToString(window_handle) + ")");
    mInputSystem = OIS::InputManager::createInputSystem(params);

    mKeyboard = static_cast<OIS::Keyboard*>(mInputSystem->createInputObject(OIS::OISKeyboard, true));
    mMouse = static_cast<OIS::Mouse*>(mInputSystem->createInputObject(OIS::OISMouse, true));

    mKeyboard->setEventCallback(this);
    mMouse->setEventCallback(this);

    // Set initial mouse clipping size
    windowResized(mWindow);
}

}
