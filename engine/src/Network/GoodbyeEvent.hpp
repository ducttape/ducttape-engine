
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_EVENT_GOODBYEEVENT
#define DUCTTAPE_ENGINE_EVENT_GOODBYEEVENT

#include <Config.hpp>

#include <Network/NetworkEvent.hpp>
#include <Network/IOPacket.hpp>

#include <memory>
#include <string>

namespace dt {

/**
  * An Event announcing the end of a Connection. Sent by the NetworkManager on disconnect.
  */
class DUCTTAPE_API GoodbyeEvent : public NetworkEvent {
public:
    /**
      * Advanced constructor.
      * @param reason The reason for the disconnect.
      */
    GoodbyeEvent(const std::string& reason = "");

    const std::string GetType() const;
    std::shared_ptr<Event> Clone() const;
    void Serialize(IOPacket& p);

    /**
      * Returns the reason for the disconnect.
      * @returns The reason for the disconnect.
      */
    const std::string& GetReason() const;

    /**
      * Sets the reason for the disconnect.
      * @param reason The reason for the disconnect.
      */
    void SetReason(const std::string& reason);

private:
    std::string mReason;    //!< The reason for the disconnect.
};

}

#endif
