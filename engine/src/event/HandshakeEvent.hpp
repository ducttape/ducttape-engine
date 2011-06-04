#ifndef DUCTTAPE_ENGINE_EVENT_HANDSHAKEEVENT
#define DUCTTAPE_ENGINE_EVENT_HANDSHAKEEVENT

#include "NetworkEvent.hpp"

namespace dt {

/**
  * Event being sent over Network when establishing a new connection.
  */
class HandshakeEvent : public NetworkEvent {
public:
    /**
      * Default constructor.
      */
    HandshakeEvent();
    const std::string GetType() const;
    Event* Clone() const;
    void Serialize(IOPacket& p);
};

}

#endif
