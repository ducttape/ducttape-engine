#include "MessageEvent.hpp"

namespace dt {

MessageEvent::MessageEvent(const std::string& message) {
    mMessage = message;
}

const std::string MessageEvent::GetType() const {
    return "DT_MESSAGEEVENT";
}

const std::string& MessageEvent::GetMessageEvent() const {
    return mMessage;
}

}
