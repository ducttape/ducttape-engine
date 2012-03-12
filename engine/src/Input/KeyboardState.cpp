
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include <Input/KeyboardState.hpp>

#include <Input/InputManager.hpp>

namespace dt {

bool KeyboardState::isKeyDown(int key_code) const {
    return InputManager::get()->getKeyboard()->isKeyDown(static_cast<OIS::KeyCode>(key_code));
}

bool KeyboardState::isAltDown() const {
    return InputManager::get()->getKeyboard()->isModifierDown(OIS::Keyboard::Alt);
}

bool KeyboardState::isShiftDown() const {
    return InputManager::get()->getKeyboard()->isModifierDown(OIS::Keyboard::Shift);
}

bool KeyboardState::isCtrlDown() const {
    return InputManager::get()->getKeyboard()->isModifierDown(OIS::Keyboard::Ctrl);
}

}
