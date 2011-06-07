#ifndef DUCTTAPE_ENGINE_EVENT_MUSICPAUSEEVENT
#define DUCTTAPE_ENGINE_EVENT_MUSICPAUSEEVENT

#include <string>

#include "Event.hpp"

namespace dt {

/**
 * Event being sent when sending a pause toggle to the music.
 */
class MusicPauseEvent : public Event {
public:
    /**
     * Default constructor.
     */
    MusicPauseEvent();
    const std::string GetType() const;
    Event* Clone() const;
};

}

#endif
