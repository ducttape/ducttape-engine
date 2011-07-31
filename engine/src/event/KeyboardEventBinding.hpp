
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_EVENT_KEYBOARDEVENTBINDING
#define DUCTTAPE_ENGINE_EVENT_KEYBOARDEVENTBINDING

#include "Config.hpp"
#include "EventBinding.hpp"
#include "input/KeyboardEvent.hpp"
#include "utils/StringManager.hpp"

namespace dt {

/**
  * An event binding that triggers when an specific
  * key was pressed.
  */
class DUCTTAPE_API KeyboardBinding : public EventBinding {
public:
    /**
      * Advanced constructor.
      * @param target The event that is being triggered.
      * @param key_code The key that triggers the event.
      */
    KeyboardBinding(Event* target, OIS::KeyCode key_code);
    bool MatchesEvent(std::shared_ptr<Event> e);

private:
    OIS::KeyCode mKeyCode; //!< The key to trigger on.
};

}

#endif
