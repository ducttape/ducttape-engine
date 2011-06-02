#ifndef DUCTTAPE_ENGINE_EVENT_GOODBYEEVENT
#define DUCTTAPE_ENGINE_EVENT_GOODBYEEVENT

#include "NetworkEvent.hpp"

namespace dt {

class GoodbyeEvent : public NetworkEvent {
public:
    GoodbyeEvent(const std::string& reason = "");
    const std::string GetType() const;
    NetworkEvent* NewInstance() const;
    void Serialize(IOPacket& p);

    const std::string& GetReason() const;
    void SetReason(const std::string& reason);
private:
    std::string mReason;
};

}

#endif
