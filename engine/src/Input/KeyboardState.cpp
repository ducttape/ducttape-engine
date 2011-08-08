
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include <Input/KeyboardState.hpp>

#include <Input/InputManager.hpp>

namespace dt {

bool KeyboardState::IsKeyDown(int key_code) const {
    return InputManager::Get()->GetKeyboard()->isKeyDown(static_cast<OIS::KeyCode>(key_code));
}

bool KeyboardState::IsAltDown() const {
    return InputManager::Get()->GetKeyboard()->isModifierDown(OIS::Keyboard::Alt);
}

bool KeyboardState::IsShiftDown() const {
    return InputManager::Get()->GetKeyboard()->isModifierDown(OIS::Keyboard::Shift);
}

bool KeyboardState::IsCtrlDown() const {
    return InputManager::Get()->GetKeyboard()->isModifierDown(OIS::Keyboard::Ctrl);
}

}
