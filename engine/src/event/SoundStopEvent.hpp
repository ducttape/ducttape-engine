#ifndef DUCTTAPE_ENGINE_EVENT_SOUNDSTOPEVENT
#define DUCTTAPE_ENGINE_EVENT_SOUNDSTOPEVENT

#include <string>

#include "Event.hpp"

namespace dt {

/**
 * Event being sent when sending a pause toggle to the sound.
 */
class SoundStopEvent : public Event {
public:
    /**
     * Default constructor.
     */
    SoundStopEvent();
    const std::string GetType() const;
    Event* Clone() const;
};

}

#endif
