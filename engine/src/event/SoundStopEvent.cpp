#include "SoundStopEvent.hpp"

namespace dt {

SoundStopEvent::SoundStopEvent() {}

const std::string SoundStopEvent::GetType() const {
   return "DT_SOUNDSTOPEVENT";
}

Event* SoundStopEvent::Clone() const {
    return new SoundStopEvent();

}

}
