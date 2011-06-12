#include "MusicPauseEvent.hpp"

namespace dt {

MusicPauseEvent::MusicPauseEvent() {}

const std::string MusicPauseEvent::GetType() const {
   return "DT_MUSICPAUSEEVENT";
}

Event* MusicPauseEvent::Clone() const {
    return new MusicPauseEvent();
}

}
