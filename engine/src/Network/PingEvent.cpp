
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include <Network/PingEvent.hpp>

namespace dt {

PingEvent::PingEvent(double timestamp, bool is_reply)
    : mIsReply(is_reply),
      mTimestamp(timestamp) {}

const QString PingEvent::GetType() const {
    return "DT_PINGEVENT";
}

std::shared_ptr<NetworkEvent> PingEvent::Clone() const {
    std::shared_ptr<NetworkEvent> ptr(new PingEvent(mTimestamp, mIsReply));
    return ptr;
}

void PingEvent::Serialize(IOPacket& p) {
    p.Stream(mTimestamp, "timestamp", 0.0);
    p.Stream(mIsReply, "is_reply", false);
}

bool PingEvent::IsReply() const {
    return mIsReply;
}

double PingEvent::GetTimestamp() const {
    return mTimestamp;
}

}
