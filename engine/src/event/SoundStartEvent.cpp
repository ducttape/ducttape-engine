#include "SoundStartEvent.hpp"

namespace dt {

SoundStartEvent::SoundStartEvent() {}

const std::string SoundStartEvent::GetType() const {
   return "DT_SOUNDSTARTEVENT";
}

Event* SoundStartEvent::Clone() const {
    return new SoundStartEvent();
}

}
