#ifndef DUCTTAPE_ENGINE_EVENT_EVENT
#define DUCTTAPE_ENGINE_EVENT_EVENT

#include <string>

#include "network/IOPacket.hpp"

namespace dt {

class Event {
public:
    Event();
    virtual const std::string GetType() const = 0;
    virtual bool IsNetworkEvent() const;
    uint32_t GetTypeID() const;

    virtual void Serialize(IOPacket& p); // TODO: make this pure in networkevent
};

}

#endif
