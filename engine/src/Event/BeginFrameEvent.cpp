
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include <Event/BeginFrameEvent.hpp>

namespace dt {

BeginFrameEvent::BeginFrameEvent(double frame_time)
    : mFrameTime(frame_time) {}

const QString BeginFrameEvent::GetType() const {
    return "DT_BEGINFRAMEEVENT";
}

std::shared_ptr<Event> BeginFrameEvent::Clone() const {
    std::shared_ptr<Event> ptr(new BeginFrameEvent(mFrameTime));
    return ptr;
}

double BeginFrameEvent::GetFrameTime() const {
    return mFrameTime;
}

}
