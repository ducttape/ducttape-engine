#include "KeyboardEvent.hpp"

namespace dt {

KeyboardEvent::KeyboardEvent(KeyboardEvent::Action action, OIS::KeyCode code, char text) {
    mAction = action;
    mCode = code;
    mText = text;
}

const std::string KeyboardEvent::GetType() const {
    return "DT_KEYBOARDEVENT";
}

Event* KeyboardEvent::Clone() const {
    return new KeyboardEvent(mAction, mCode, mText);
}

OIS::KeyCode KeyboardEvent::GetCode() const {
    return mCode;
}

KeyboardEvent::Action KeyboardEvent::GetAction() const {
    return mAction;
}

char KeyboardEvent::GetText() const {
    return mText;
}

}
