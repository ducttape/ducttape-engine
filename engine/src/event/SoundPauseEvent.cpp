#include "SoundPauseEvent.hpp"

namespace dt {

SoundPauseEvent::SoundPauseEvent() {}

const std::string SoundPauseEvent::GetType() const {
   return "DT_SOUNDPAUSEEVENT";
}

Event* SoundPauseEvent::Clone() const {
    return new SoundPauseEvent();
}

}
