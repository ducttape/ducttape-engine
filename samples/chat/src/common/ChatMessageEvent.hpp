#ifndef DUCTTAPE_SAMPLE_CHAT_COMMON_CHATMESSAGEEVENT
#define DUCTTAPE_SAMPLE_CHAT_COMMON_CHATMESSAGEEVENT

#include "event/NetworkEvent.hpp"

class ChatMessageEvent : public dt::NetworkEvent {
public:
    ChatMessageEvent(const std::string& message, const std::string& sender);
    const std::string GetType() const;

    dt::Event* Clone() const;
    void Serialize(dt::IOPacket& p);

    const std::string& GetMessage() const;
    const std::string& GetSenderNick() const;
protected:
    std::string mMessage;
    std::string mSenderNick;
};

#endif
