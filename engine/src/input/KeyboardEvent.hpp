
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_INPUT_KEYBOARDEVENT
#define DUCTTAPE_ENGINE_INPUT_KEYBOARDEVENT

#include "event/Event.hpp"

#include <OIS/OISKeyboard.h>

namespace dt {

/**
  * Event triggered on MousePressed, MouseReleased and MouseMoved
  */
class KeyboardEvent : public Event {
public:
    enum Action {
        PRESSED,
        RELEASED
    };

    KeyboardEvent(Action action, OIS::KeyCode code, char text);
    const std::string GetType() const;
    Event* Clone() const;

    OIS::KeyCode GetCode() const;
    Action GetAction() const;
    char GetText() const;

private:
    OIS::KeyCode mCode;
    Action mAction;
    char mText;

};

}

#endif
