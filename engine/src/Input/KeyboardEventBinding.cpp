
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------


namespace dt {

KeyboardBinding::KeyboardBinding(Event* target, OIS::KeyCode key_code)
    : EventBinding(target),
      mKeyCode(key_code) {}

bool KeyboardBinding::MatchesEvent(std::shared_ptr<Event> e) {
    if(e->GetType() == "DT_KEYBOARDEVENT") {
        std::shared_ptr<KeyboardEvent> k = std::dynamic_pointer_cast<KeyboardEvent>(e);
        return k->GetCode() == mKeyCode;
    }
    return false;
}

}
