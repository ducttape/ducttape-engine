
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_EVENT_HANDSHAKEEVENT
#define DUCTTAPE_ENGINE_EVENT_HANDSHAKEEVENT


namespace dt {

/**
  * Event being sent over Network when establishing a new connection.
  */
class DUCTTAPE_API HandshakeEvent : public NetworkEvent {
public:
    /**
      * Default constructor.
      */
    HandshakeEvent();
    const std::string GetType() const;
    std::shared_ptr<Event> Clone() const;
    void Serialize(IOPacket& p);
};

}

#endif
