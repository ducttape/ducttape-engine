
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_EVENT_MUSICPAUSEEVENT
#define DUCTTAPE_ENGINE_EVENT_MUSICPAUSEEVENT

#include <string>

#include "Config.hpp"
#include "Event.hpp"

namespace dt {

/**
 * Event being sent when sending a pause toggle to the music.
 */
class DUCTTAPE_API MusicPauseEvent : public Event {
public:
    /**
     * Default constructor.
     */
    MusicPauseEvent();
    const std::string GetType() const;
    std::shared_ptr<Event> Clone() const;
};

}

#endif
