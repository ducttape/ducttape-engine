
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include <Input/KeyboardEvent.hpp>

namespace dt {

KeyboardEvent::KeyboardEvent(KeyboardEvent::Action action, OIS::KeyCode code, char text)
    : mCode(code),
      mAction(action),
      mText(text) {}

const QString KeyboardEvent::GetType() const {
    return "DT_KEYBOARDEVENT";
}

std::shared_ptr<Event> KeyboardEvent::Clone() const {
    std::shared_ptr<Event> ptr(new KeyboardEvent(mAction, mCode, mText));
    return ptr;
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
