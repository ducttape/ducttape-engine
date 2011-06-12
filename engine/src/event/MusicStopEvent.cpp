#include "MusicStopEvent.hpp"

namespace dt {

MusicStopEvent::MusicStopEvent() {}

const std::string MusicStopEvent::GetType() const {
   return "DT_MUSICSTOPEVENT";
}

Event* MusicStopEvent::Clone() const {
    return new MusicStopEvent();

}

}
