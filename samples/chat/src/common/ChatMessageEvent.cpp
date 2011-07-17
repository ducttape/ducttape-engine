
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

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

const std::string& ChatMessageEvent::GetMessageEvent() const {
    return mMessage;
}

const std::string& ChatMessageEvent::GetSenderNick() const {
    return mSenderNick;
}
