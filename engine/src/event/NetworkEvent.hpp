#ifndef DUCTTAPE_ENGINE_EVENT_NETWORKEVENT
#define DUCTTAPE_ENGINE_EVENT_NETWORKEVENT

#include <vector>

#include "Event.hpp"

namespace dt {

/**
  * Abstract base class for all Events supposed to be sent via network.
  */
class NetworkEvent : public Event {
public:
    /**
      * Default constructor.
      */
    NetworkEvent();
    virtual const std::string GetType() const = 0;
    bool IsNetworkEvent() const;

    /**
      * Creates a new instance of the same Event type.
      * @see Factory Pattern
      * @returns A new instance of the same Event type.
      */
    virtual NetworkEvent* NewInstance() const = 0; // only network events need to be created from prototypes
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
protected:
    std::vector<uint16_t> mRecipients;  //!< The list of recipients for this NetworkEvent.
};

}

#endif
