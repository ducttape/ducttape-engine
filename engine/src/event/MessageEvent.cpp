#include "MessageEvent.hpp"

namespace dt {

MessageEvent::MessageEvent(const std::string& message) {
    mMessage = message;
}

const std::string MessageEvent::GetType() const {
    return "event:type:messageevent";
}

const std::string& MessageEvent::GetMessage() const {
    return mMessage;
}

}
