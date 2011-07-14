#ifndef DUCTTAPE_ENGINE_EVENT_SOUNDSTARTEVENT
#define DUCTTAPE_ENGINE_EVENT_SOUNDSTARTEVENT

#include <string>

#include "Event.hpp"

namespace dt {

/**
 * Event being sent when sending a pause toggle to the sound.
 */
class SoundStartEvent : public Event {
public:
    /**
     * Default constructor.
     */
    SoundStartEvent();
    const std::string GetType() const;
    Event* Clone() const;
};

}

#endif
