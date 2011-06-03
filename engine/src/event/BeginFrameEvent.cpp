#include "BeginFrameEvent.hpp"

namespace dt {

BeginFrameEvent::BeginFrameEvent(uint32_t frame_time) {
    mFrameTime = frame_time;
}

const std::string BeginFrameEvent::GetType() const {
    return "DT_BEGINFRAMEEVENT";
}

uint32_t BeginFrameEvent::GetFrameTime() const {
    return mFrameTime;
}

}
