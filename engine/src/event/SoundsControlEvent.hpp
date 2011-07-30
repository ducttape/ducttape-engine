
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_EVENT_SOUNDSCONTROLEVENT
#define DUCTTAPE_ENGINE_EVENT_SOUNDSCONTROLEVENT

#include <string>

#include "Config.hpp"
#include "Event.hpp"

namespace dt {

/**
 * Event being sent when sending a play to all sounds.
 */
class DUCTTAPE_API SoundsControlEvent : public Event {
public:
    enum Action {
        PLAY = 0,
        PAUSE = 1,
        STOP = 2
    };

    /**
     * Advanced constructor.
     * @param action The action to perform with all sounds.
     */
    SoundsControlEvent(Action action);
    const std::string GetType() const;
    std::shared_ptr<Event> Clone() const;

    Action GetAction() const;

private:
    Action mAction;
};

}

#endif
