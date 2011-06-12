#include "MusicStartEvent.hpp"

namespace dt {

MusicStartEvent::MusicStartEvent() {}

const std::string MusicStartEvent::GetType() const {
   return "DT_MUSICSTARTEVENT";
}

Event* MusicStartEvent::Clone() const {
    return new MusicStartEvent();
}

}
