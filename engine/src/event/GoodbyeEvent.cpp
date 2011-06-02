#include "GoodbyeEvent.hpp"

namespace dt {

GoodbyeEvent::GoodbyeEvent(const std::string& reason) {
    mReason = reason;
}

const std::string GoodbyeEvent::GetType() const {
    return "event:type:network:goodbyeevent";
}

NetworkEvent* GoodbyeEvent::NewInstance() const {
    return new GoodbyeEvent();
}

void GoodbyeEvent::Serialize(IOPacket& p) {
    p & mReason;
}

const std::string& GoodbyeEvent::GetReason() const {
    return mReason;
}

void GoodbyeEvent::SetReason(const std::string& reason) {
    mReason = reason;
}

}
