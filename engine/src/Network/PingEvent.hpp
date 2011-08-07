
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_EVENT_PINGEVENT
#define DUCTTAPE_ENGINE_EVENT_PINGEVENT

#include <Config.hpp>

#include <Event/Event.hpp>
#include <Network/NetworkEvent.hpp>

#include <QString>

#include <memory>

namespace dt {

/**
  * Event for measuring and calculating round-trip-time (ping).
  */
class DUCTTAPE_API PingEvent : public NetworkEvent {
public:
    /**
      * Advanced constructor.
      * @param timestamp The time the ping was sent.
      * @param is_reply Whether this ping is a reply.
      */
    PingEvent(double timestamp, bool is_reply = false);
    const QString GetType() const;

    std::shared_ptr<Event> Clone() const;
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
    double GetTimestamp() const;

protected:
    bool mIsReply;          //!< Whether this ping is a reply.
    double mTimestamp;    //!< The time the ping was sent.
};

}

#endif
