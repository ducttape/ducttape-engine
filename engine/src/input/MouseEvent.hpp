
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_INPUT_MOUSEEVENT
#define DUCTTAPE_ENGINE_INPUT_MOUSEEVENT

#include "event/Event.hpp"

#include <OIS/OISMouse.h>

namespace dt {

/**
  * Event triggered on MousePressed, MouseReleased and MouseMoved
  */
class MouseEvent : public Event {
public:
    enum Action {
        MOVED,
        PRESSED,
        RELEASED
    };

    MouseEvent(Action action, OIS::MouseState state, OIS::MouseButtonID button = OIS::MB_Left);
    const std::string GetType() const;
    Event* Clone() const;

    const OIS::MouseState& GetMouseState() const;
    Action GetAction() const;
    OIS::MouseButtonID GetButton() const;

private:
    OIS::MouseState mState;
    Action mAction;
    OIS::MouseButtonID mButton;

};

}

#endif
