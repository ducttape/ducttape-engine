#include "KeyboardEventBinding.hpp"

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
