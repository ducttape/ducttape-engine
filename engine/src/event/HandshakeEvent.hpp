#ifndef DUCTTAPE_ENGINE_EVENT_HANDSHAKEEVENT
#define DUCTTAPE_ENGINE_EVENT_HANDSHAKEEVENT

#include "NetworkEvent.hpp"

namespace dt {

class HandshakeEvent : public NetworkEvent {
public:
    HandshakeEvent();
    const std::string GetType() const;
    NetworkEvent* NewInstance() const;
    void Serialize(IOPacket& p);
};

}

#endif
