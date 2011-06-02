#ifndef DUCTTAPE_ENGINE_EVENT_NETWORKEVENT
#define DUCTTAPE_ENGINE_EVENT_NETWORKEVENT

#include <vector>

#include "Event.hpp"

namespace dt {

class NetworkEvent : public Event {
public:
    NetworkEvent();
    virtual const std::string GetType() const = 0;
    bool IsNetworkEvent() const;

    virtual NetworkEvent* NewInstance() const = 0; // only network events need to be created from prototypes
    virtual void Serialize(IOPacket& p) = 0; // pure virtual, because ALL network events need to be serializable

    void AddRecipient(uint16_t id);
    void RemoveRecipient(uint16_t id);
    void ClearRecipients();
    const std::vector<uint16_t>& GetRecipients() const;
    bool HasRecipient(uint16_t id);
protected:
    std::vector<uint16_t> mRecipients;
};

}

#endif
