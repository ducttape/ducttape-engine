#include "Event.hpp"

#include "Root.hpp"

namespace dt {

Event::Event() {}

bool Event::IsNetworkEvent() const {
    return false;
}

uint32_t Event::GetTypeID() const {
    return Root::get_mutable_instance().GetStringManager()->Get(GetType());
}

bool Event::TranslateFrom(Event* raw_event) {
    if(raw_event->GetType() == GetType()) {
        // Infinite loop != infinite fun.
        // Explanation: The event is supposed to be translated from another event of the same type.
        // The BindingsManager would receive this event - and translate it again.
        // Thus we would be stuck in an infinite loop.
        return false;
    }
    return true;
}

}
