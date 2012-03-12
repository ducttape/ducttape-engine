
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include <Input/MouseState.hpp>

#include <Input/InputManager.hpp>

namespace dt {

bool MouseState::isButtonDown(int button) {
    return InputManager::get()->getMouse()->getMouseState().buttonDown(static_cast<OIS::MouseButtonID>(button));
}

bool MouseState::isLeftButtonDown() {
    return isButtonDown(OIS::MB_Left);
}

bool MouseState::isRightButtonDown() {
    return isButtonDown(OIS::MB_Right);
}

bool MouseState::isMiddleButtonDown() {
    return isButtonDown(OIS::MB_Middle);
}

int MouseState::getX() {
    return InputManager::get()->getMouse()->getMouseState().X.abs;
}

int MouseState::getY() {
    return InputManager::get()->getMouse()->getMouseState().Y.abs;
}

}
