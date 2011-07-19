#include "MouseEvent.hpp"

namespace dt {

MouseEvent::MouseEvent(MouseEvent::Action action, OIS::MouseState state, OIS::MouseButtonID button) {
    mAction = action;
    mState = state;
    mButton = button;
}

const std::string MouseEvent::GetType() const {
    return "DT_MOUSEEVENT";
}

Event* MouseEvent::Clone() const {
    return new MouseEvent(mAction, mState, mButton);
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
