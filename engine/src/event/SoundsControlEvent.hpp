#ifndef DUCTTAPE_ENGINE_EVENT_SOUNDSCONTROLEVENT
#define DUCTTAPE_ENGINE_EVENT_SOUNDSCONTROLEVENT

#include <string>

#include "Event.hpp"

namespace dt {

/**
 * Event being sent when sending a play to all sounds.
 */
class SoundsControlEvent : public Event {
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
    Event* Clone() const;

    Action GetAction() const;

private:
    Action mAction;
};

}

#endif
