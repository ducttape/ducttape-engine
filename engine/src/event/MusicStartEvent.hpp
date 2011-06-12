#ifndef DUCTTAPE_ENGINE_EVENT_MUSICSTARTEVENT
#define DUCTTAPE_ENGINE_EVENT_MUSICSTARTEVENT

#include <string>

#include "Event.hpp"

namespace dt {

/**
 * Event being sent when sending a pause toggle to the music.
 */
class MusicStartEvent : public Event {
public:
    /**
     * Default constructor.
     */
    MusicStartEvent();
    const std::string GetType() const;
    Event* Clone() const;
};

}

#endif
