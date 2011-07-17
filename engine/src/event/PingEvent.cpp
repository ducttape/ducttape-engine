#include "PingEvent.hpp"

namespace dt {

PingEvent::PingEvent(uint32_t timestamp, bool is_reply) {
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

uint32_t PingEvent::GetTimestamp() const {
    return mTimestamp;
}

}
