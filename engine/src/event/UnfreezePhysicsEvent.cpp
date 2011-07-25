#include "UnfreezePhysicsEvent.hpp"

namespace dt {

UnfreezePhysicsEvent::UnfreezePhysicsEvent() {}

const std::string UnfreezePhysicsEvent::GetType() const {
    return "DT_UNFREEZEPHYSICSEVENT";
}

std::shared_ptr<Event> UnfreezePhysicsEvent::Clone() const {
    std::shared_ptr<Event> ptr(new UnfreezePhysicsEvent());
    return ptr;
}

}
