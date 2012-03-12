
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include <Network/GoodbyeEvent.hpp>

namespace dt {

GoodbyeEvent::GoodbyeEvent(const QString reason)
    : mReason(reason) {}

const QString GoodbyeEvent::getType() const {
    return "DT_GOODBYEEVENT";
}

std::shared_ptr<NetworkEvent> GoodbyeEvent::clone() const {
    std::shared_ptr<NetworkEvent> ptr(new GoodbyeEvent(mReason));
    return ptr;
}

void GoodbyeEvent::serialize(IOPacket& p) {
    p.stream(mReason, "reason", "");
}

const QString GoodbyeEvent::getReason() const {
    return mReason;
}

void GoodbyeEvent::setReason(const QString reason) {
    mReason = reason;
}

}
