
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include "BeginFrameEvent.hpp"

namespace dt {

BeginFrameEvent::BeginFrameEvent(uint32_t frame_time) {
    mFrameTime = frame_time;
}

const std::string BeginFrameEvent::GetType() const {
    return "DT_BEGINFRAMEEVENT";
}

Event* BeginFrameEvent::Clone() const {
    return new BeginFrameEvent(mFrameTime);
}

uint32_t BeginFrameEvent::GetFrameTime() const {
    return mFrameTime;
}

}
