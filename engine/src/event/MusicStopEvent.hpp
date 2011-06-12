#ifndef DUCTTAPE_ENGINE_EVENT_MUSICSTOPEVENT
#define DUCTTAPE_ENGINE_EVENT_MUSICSTOPEVENT

#include <string>

#include "Event.hpp"

namespace dt {

/**
 * Event being sent when sending a pause toggle to the music.
 */
class MusicStopEvent : public Event {
public:
    /**
     * Default constructor.
     */
    MusicStopEvent();
    const std::string GetType() const;
    Event* Clone() const;
};

}

#endif
