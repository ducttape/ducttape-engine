
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_INPUT_KEYBOARDEVENT
#define DUCTTAPE_ENGINE_INPUT_KEYBOARDEVENT

#include <Config.hpp>

#include <Event/Event.hpp>

#include <OIS.h>

#include <QString>

#include <memory>

namespace dt {

/**
  * Event triggered on MousePressed, MouseReleased and MouseMoved
  */
class DUCTTAPE_API KeyboardEvent : public Event {
public:
    /**
      * Action performed that triggered the event.
      */
    enum Action {
        PRESSED,    //!< The key has been pressed.
        RELEASED    //!< The key has been released.
    };

    /**
      * Advanced constructor.
      * @param action The action that triggered the event.
      * @param code The code of the key pressed.
      * @param text The character the pressed key is assigned to.
      */
    KeyboardEvent(Action action, OIS::KeyCode code, char text);
    const QString GetType() const;
    std::shared_ptr<Event> Clone() const;

    /**
      * Returns the code of the key pressed.
      * @returns The code of the key pressed.
      */
    OIS::KeyCode GetCode() const;


    /**
      * Returns the action that triggered the event.
      * @returns The action that triggered the event.
      */
    Action GetAction() const;

    /**
      * Returns the character the pressed key is assigned to.
      * @returns The character the pressed key is assigned to.
      */
    char GetText() const;

private:
    OIS::KeyCode mCode; //!< The key code of the key pressed.
    Action mAction;     //!< The action that triggered the event.
    char mText;         //!< The character the pressed key is assigned to.

};

}

#endif
