#ifndef DUCTTAPE_ENGINE_EVENT_KEYBOARDEVENTBINDING
#define DUCTTAPE_ENGINE_EVENT_KEYBOARDEVENTBINDING

#include "EventBinding.hpp"
#include "input/KeyboardEvent.hpp"
#include "utils/StringManager.hpp"

namespace dt {

/**
  * An event binding that triggers when an specific
  * key was pressed.
  */
class KeyboardBinding : public EventBinding {
public:
    /**
      * Advanced constructor.
      * @param target The event that is being triggered.
      * @param key_code The key that triggers the event.
      */
    KeyboardBinding(Event* target, OIS::KeyCode key_code);
    bool MatchesEvent(std::shared_ptr<Event> e);

private:
    OIS::KeyCode mKeyCode;

};

}

#endif
