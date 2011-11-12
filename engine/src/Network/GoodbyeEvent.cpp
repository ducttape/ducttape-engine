
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include <Network/GoodbyeEvent.hpp>

namespace dt {

GoodbyeEvent::GoodbyeEvent(const QString& reason)
    : mReason(reason) {}

const QString GoodbyeEvent::GetType() const {
    return "DT_GOODBYEEVENT";
}

std::shared_ptr<NetworkEvent> GoodbyeEvent::Clone() const {
    std::shared_ptr<NetworkEvent> ptr(new GoodbyeEvent(mReason));
    return ptr;
}

void GoodbyeEvent::Serialize(IOPacket& p) {
    p.Stream(mReason, "reason", "");
}

const QString& GoodbyeEvent::GetReason() const {
    return mReason;
}

void GoodbyeEvent::SetReason(const QString& reason) {
    mReason = reason;
}

}
