#include "HandshakeEvent.hpp"

namespace dt {

HandshakeEvent::HandshakeEvent() {}

const std::string HandshakeEvent::GetType() const {
    return "event:type:network:handshakeevent";
}

NetworkEvent* HandshakeEvent::NewInstance() const {
    return new HandshakeEvent();
}

void HandshakeEvent::Serialize(IOPacket& p) {
    // do nothing, we have no data
}

}
