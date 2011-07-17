#ifndef DUCTTAPE_ENGINE_EVENT_PINGEVENT
#define DUCTTAPE_ENGINE_EVENT_PINGEVENT

#include "NetworkEvent.hpp"

namespace dt {

/**
  * Event for measuring and calculating round-trip-time (ping).
  */
class PingEvent : public NetworkEvent {
public:
    /**
      * Advanced constructor.
      * @param timestamp The time the ping was sent.
      * @param is_reply Whether this ping is a reply.
      */
    PingEvent(uint32_t timestamp, bool is_reply = false);
    const std::string GetType() const;

    Event* Clone() const;
    void Serialize(IOPacket& p);

    /**
      * Returns whether this ping is a reply.
      * @returns Whether this ping is a reply.
      */
    bool IsReply() const;

    /**
      * Returns the time the ping was sent.
      * @returns The time the ping was sent.
      */
    uint32_t GetTimestamp() const;
protected:
    bool mIsReply;          //!< Whether this ping is a reply.
    uint32_t mTimestamp;    //!< The time the ping was sent.
};

}

#endif
