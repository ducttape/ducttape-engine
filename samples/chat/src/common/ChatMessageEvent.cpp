#include "ChatMessageEvent.hpp"

ChatMessageEvent::ChatMessageEvent(const std::string& message, const std::string& sender) {
    mMessage = message;
    mSenderNick = sender;
}

const std::string ChatMessageEvent::GetType() const {
    return "CHATMESSAGEEVENT";
}

dt::Event* ChatMessageEvent::Clone() const {
    return new ChatMessageEvent(mMessage, mSenderNick);
}

void ChatMessageEvent::Serialize(dt::IOPacket& p) {
    p & mMessage;
    p & mSenderNick;
}

const std::string& ChatMessageEvent::GetMessage() const {
    return mMessage;
}

const std::string& ChatMessageEvent::GetSenderNick() const {
    return mSenderNick;
}
