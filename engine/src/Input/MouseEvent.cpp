
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include <Input/MouseEvent.hpp>

namespace dt {

MouseEvent::MouseEvent(MouseEvent::Action action, OIS::MouseState state, OIS::MouseButtonID button)
    : mState(state),
      mAction(action),
      mButton(button) {}

const QString MouseEvent::GetType() const {
    return "DT_MOUSEEVENT";
}

std::shared_ptr<Event> MouseEvent::Clone() const {
    std::shared_ptr<Event> ptr(new MouseEvent(mAction, mState, mButton));
    return ptr;
}

const OIS::MouseState& MouseEvent::GetMouseState() const {
    return mState;
}

MouseEvent::Action MouseEvent::GetAction() const {
    return mAction;
}

OIS::MouseButtonID MouseEvent::GetButton() const {
    return mButton;
}

}
