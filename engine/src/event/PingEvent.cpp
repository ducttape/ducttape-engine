
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include "PingEvent.hpp"

namespace dt {

PingEvent::PingEvent(double timestamp, bool is_reply) {
    mTimestamp = timestamp;
    mIsReply = is_reply;
}

const std::string PingEvent::GetType() const {
    return "DT_PINGEVENT";
}

Event* PingEvent::Clone() const {
    return new PingEvent(mTimestamp, mIsReply);
}

void PingEvent::Serialize(IOPacket& p) {
    p & mTimestamp;
    p & mIsReply;
}

bool PingEvent::IsReply() const {
    return mIsReply;
}

double PingEvent::GetTimestamp() const {
    return mTimestamp;
}

}
