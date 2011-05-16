#ifndef DUCTTAPE_ENGINE_STATES_STATE
#define DUCTTAPE_ENGINE_STATES_STATE

#include "event/Event.hpp"
#include "event/EventListener.hpp"

namespace dt {

class State : public EventListener {
public:
    State();

    virtual const std::string GetType() = 0;
    uint32_t GetTypeID();

    virtual void OnUpdate(float time_delta) = 0;
    virtual void OnEvent(Event& event);

    void Update(float time_delta);
    void HandleEvent(Event& event);
};

}

#endif
