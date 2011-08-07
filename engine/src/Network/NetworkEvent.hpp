
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_EVENT_NETWORKEVENT
#define DUCTTAPE_ENGINE_EVENT_NETWORKEVENT

#include <Config.hpp>

#include <Event/Event.hpp>
#include <Network/IOPacket.hpp>

#include <QString>

#include <cstdint>
#include <memory>
#include <vector>

namespace dt {

/**
  * Abstract base class for all Events supposed to be sent via network.
  */
class DUCTTAPE_API NetworkEvent : public virtual Event {
public:
    /**
      * Default constructor.
      */
    NetworkEvent();
    virtual const QString GetType() const = 0;
    bool IsNetworkEvent() const;

    /**
      * Creates a new instance of the same Event type.
      * @see Factory Pattern
      * @returns A new instance of the same Event type.
      */
    virtual std::shared_ptr<Event> Clone() const = 0;

    /**
      * Serializes the event into/from an sf::Packet using the wrapper IOPacket.
      * @see IOPacket
      * @param p The IOPacket to stream from/into.
      */
    virtual void Serialize(IOPacket& p) = 0; // pure virtual, because ALL network events need to be serializable

    /**
      * Adds a recipient to the list of recipients.
      * @param id The ID of the recipient in the ConnectionsManager.
      * @see ConnectionsManager
      */
    void AddRecipient(uint16_t id);

    /**
      * Removes a recipient from the list of recipients.
      * @param id The ID of the recipient in the ConnectionsManager.
      * @see ConnectionsManager
      */
    void RemoveRecipient(uint16_t id);

    /**
      * Clears the list of recipients.
      */
    void ClearRecipients();

    /**
      * Returns the list of recipients for this NetworkEvent.
      * @returns The list of recipients.
      */
    const std::vector<uint16_t>& GetRecipients() const;

    /**
      * Returns whether the recipient is in the list of recipients.
      * @param id The ID of the recipient in the ConnectionsManager.
      * @returns True if the recipient is part of the list, otherwise false.
      */
    bool HasRecipient(uint16_t id);

    /**
      * Returns whether this event should only be handled locally.
      * @see NetworkEvent::IsLocalEvent(bool is_local_event);
      * @returns True if this event should only be handled locally.
      */
    bool IsLocalEvent() const;

    /**
      * Sets whether this event should only be handled locally.
      * @param is_local_event Whether this event should only be handled locally.
      * @see NetworkEvent::IsLocalEvent();
      */
    void IsLocalEvent(bool is_local_event);

    /**
      * Returns the Connection ID of the Connection this Event was received from.
      * @returns The Connection ID of the sender or 0 if it has not yet been sent.
      */
    uint16_t GetSenderID() const;

    /**
      * Sets the Connection ID of the sender.
      * @param id The new Connection ID.
      */
    void SetSenderID(uint16_t id);

protected:
    std::vector<uint16_t> mRecipients;  //!< The list of recipients for this NetworkEvent.
    uint16_t mSenderID;                 //!< The Connection ID of the sender. This is being set when the Event is received in the NetworkManager.
    bool mIsLocalEvent;                 //!< Whether this event should only be handled locally.
};

}

#endif
