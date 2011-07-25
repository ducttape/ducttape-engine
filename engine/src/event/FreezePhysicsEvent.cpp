#include "FreezePhysicsEvent.hpp"

namespace dt {

FreezePhysicsEvent::FreezePhysicsEvent() {}

const std::string FreezePhysicsEvent::GetType() const {
    return "DT_FREEZEPHYSICS";
}

std::shared_ptr<Event> FreezePhysicsEvent::Clone() const {
    std::shared_ptr<Event> ptr(new FreezePhysicsEvent());
    return ptr;
}

}
