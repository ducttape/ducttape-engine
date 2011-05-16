#ifndef DUCTTAPE_ENGINE_EVENT_EVENT
#define DUCTTAPE_ENGINE_EVENT_EVENT

#include <string>

namespace dt {

class Event {
public:
    Event();
    virtual const std::string GetType() const = 0;
    uint32_t GetTypeID() const;

    virtual void Serialize(); // TODO: make this pure in networkevent
};

}

#endif
