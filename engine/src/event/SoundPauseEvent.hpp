#ifndef DUCTTAPE_ENGINE_EVENT_SOUNDPAUSEEVENT
#define DUCTTAPE_ENGINE_EVENT_SOUNDPAUSEEVENT

#include <string>

#include "Event.hpp"

namespace dt {

/**
 * Event being sent when sending a pause toggle to the sound.
 */
class SoundPauseEvent : public Event {
public:
    /**
     * Default constructor.
     */
    SoundPauseEvent();
    const std::string GetType() const;
    Event* Clone() const;
};

}

#endif
