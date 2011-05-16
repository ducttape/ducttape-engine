#include "Event.hpp"

#include "Root.hpp"

namespace dt {

Event::Event() {}

uint32_t Event::GetTypeID() const {
    return Root::get_mutable_instance().GetStringManager()->Get(GetType());
}

void Event::Serialize() {}

}
