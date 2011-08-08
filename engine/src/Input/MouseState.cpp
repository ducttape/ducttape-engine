
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include <Input/MouseState.hpp>

#include <Input/InputManager.hpp>

namespace dt {

bool MouseState::IsButtonDown(int button) {
    return InputManager::Get()->GetMouse()->getMouseState().buttonDown(static_cast<OIS::MouseButtonID>(button));
}

bool MouseState::IsLeftButtonDown() {
    return IsButtonDown(OIS::MB_Left);
}

bool MouseState::IsRightButtonDown() {
    return IsButtonDown(OIS::MB_Right);
}

bool MouseState::IsMiddleButtonDown() {
    return IsButtonDown(OIS::MB_Middle);
}

int MouseState::GetX() {
    return InputManager::Get()->GetMouse()->getMouseState().X.abs;
}

int MouseState::GetY() {
    return InputManager::Get()->GetMouse()->getMouseState().Y.abs;
}

}
