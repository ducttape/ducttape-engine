#include "WindowClosedEvent.hpp"

namespace dt {

WindowClosedEvent::WindowClosedEvent() {}

const std::string WindowClosedEvent::GetType() const {
    return "DT_WINDOWCLOSEDEVENT";
}

std::shared_ptr<Event> WindowClosedEvent::Clone() const {
    std::shared_ptr<Event> ptr(new WindowClosedEvent());
    return ptr;
}

}
