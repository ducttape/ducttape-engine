
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_INPUT_MOUSEEVENT
#define DUCTTAPE_ENGINE_INPUT_MOUSEEVENT

#include <Config.hpp>

#include <Event/Event.hpp>

#include <OIS.h>

#include <QString>

#include <memory>

namespace dt {

/**
  * Event triggered on MousePressed, MouseReleased and MouseMoved.
  */
class DUCTTAPE_API MouseEvent : public Event {
public:
    /**
      * The action that caused the MouseEvent.
      */
    enum Action {
        MOVED,      //!< The mouse has moved.
        PRESSED,    //!< The mouse has been pressed.
        RELEASED    //!< The mouse has been released.
    };

    /**
      * Advanced constructor.
      * @param action The action that caused the MouseEvent.
      * @param state The current mouse state.
      * @param button The mouse button that caused the event. For MOVED, this will always be OIS::MB_Left.
      */
    MouseEvent(Action action, OIS::MouseState state, OIS::MouseButtonID button = OIS::MB_Left);
    const QString GetType() const;
    std::shared_ptr<Event> Clone() const;

    /**
      * Returns the current mouse state.
      * @returns The current mouse state.
      */
    const OIS::MouseState& GetMouseState() const;

    /**
      * Returns the action that caused the MouseEvent.
      * @returns The action that caused the MouseEvent.
      */
    Action GetAction() const;

    /**
      * Returns the mouse button that caused the event. For MOVED, this will always be OIS::MB_Left.
      * @returns The mouse button that caused the event.
      */
    OIS::MouseButtonID GetButton() const;

private:
    OIS::MouseState mState;         //!< The current mouse state.
    Action mAction;                 //!< The action that caused the MouseEvent.
    OIS::MouseButtonID mButton;     //!< The mouse butotn that caused the event.

};

}

#endif
