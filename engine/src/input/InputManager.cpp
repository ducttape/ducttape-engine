#include "InputManager.hpp"

#include "Root.hpp"

namespace dt {

InputManager::InputManager() {
    mInputSystem = nullptr;
}

void InputManager::Initialize(Ogre::RenderWindow* window) {
    if(mInputSystem != nullptr) {
        Logger::Get().Warning("Input system already initialized. Aborting.");
        return;
    }

    mWindow = window;

    OIS::ParamList params;

    // getting window handle
    size_t window_handle = 0;
    mWindow->getCustomAttribute("WINDOW", &window_handle);
    params.insert(std::make_pair(std::string("WINDOW"), tostr(window_handle)));

    Logger::Get().Info("Initializing input system (Window: " + tostr(window_handle) + ")");
    mInputSystem = OIS::InputManager::createInputSystem(params);

    mKeyboard = static_cast<OIS::Keyboard*>(mInputSystem->createInputObject(OIS::OISKeyboard, false));
    mMouse = static_cast<OIS::Mouse*>(mInputSystem->createInputObject(OIS::OISMouse, false));
}

void InputManager::Deinitialize() {
    if(mInputSystem != nullptr) {
        mInputSystem->destroyInputObject(mMouse);
        mInputSystem->destroyInputObject(mKeyboard);
        OIS::InputManager::destroyInputSystem(mInputSystem);
        mInputSystem = nullptr;
    }
}

void InputManager::Capture() {
    mMouse->capture();
    mKeyboard->capture();
}

bool InputManager::keyPressed(const OIS::KeyEvent& event) {
    Root::get_mutable_instance().GetEventManager()->HandleEvent(new KeyboardEvent(KeyboardEvent::PRESSED, event.key, event.text));
    return true;
}

bool InputManager::keyReleased(const OIS::KeyEvent& event) {
    Root::get_mutable_instance().GetEventManager()->HandleEvent(new KeyboardEvent(KeyboardEvent::RELEASED, event.key, event.text));
    return true;
}

bool InputManager::mouseMoved(const OIS::MouseEvent& event) {
    Root::get_mutable_instance().GetEventManager()->HandleEvent(new MouseEvent(MouseEvent::MOVED, event.state));
    return true;
}

bool InputManager::mousePressed(const OIS::MouseEvent& event, OIS::MouseButtonID button) {
    Root::get_mutable_instance().GetEventManager()->HandleEvent(new MouseEvent(MouseEvent::PRESSED, event.state, button));
    return true;
}

bool InputManager::mouseReleased(const OIS::MouseEvent& event, OIS::MouseButtonID button) {
    Root::get_mutable_instance().GetEventManager()->HandleEvent(new MouseEvent(MouseEvent::RELEASED, event.state, button));
    return true;
}

void InputManager::windowResized(Ogre::RenderWindow* window) {
    // update the mouse capture width / height

    unsigned int width, height, depth;
    int left, top;
    window->getMetrics(width, height, depth, left, top);

    const OIS::MouseState &ms = mMouse->getMouseState();
    ms.width = width;
    ms.height = height;
}

void InputManager::windowClosed(Ogre::RenderWindow* window) {
    // Only close for window that created OIS
    if(window == mWindow) {
        Root::get_mutable_instance().GetEventManager()->HandleEvent(new WindowClosedEvent());
    }
}

}
