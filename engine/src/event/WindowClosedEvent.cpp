#include "WindowClosedEvent.hpp"

namespace dt {

WindowClosedEvent::WindowClosedEvent() {}

const std::string WindowClosedEvent::GetType() const {
    return "DT_WINDOWCLOSEDEVENT";
}

Event* WindowClosedEvent::Clone() const {
    return new WindowClosedEvent();
}

}
