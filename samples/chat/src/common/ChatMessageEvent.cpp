
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include "ChatMessageEvent.hpp"

ChatMessageEvent::ChatMessageEvent(const QString message, const QString sender) /*:
    MessageEvent(message)*/ {
    mSenderNick = sender;
    mMessage = message;
}

const QString ChatMessageEvent::getType() const {
    return "CHATMESSAGEEVENT";
}

std::shared_ptr<dt::NetworkEvent> ChatMessageEvent::clone() const {
    std::shared_ptr<dt::NetworkEvent> ptr(new ChatMessageEvent(mMessage, mSenderNick));
    return ptr;
}

void ChatMessageEvent::serialize(dt::IOPacket& p) {
    p.stream(mMessage, "message");
    p.stream(mSenderNick, "sender_nick");
}

const QString ChatMessageEvent::getSenderNick() const {
    return mSenderNick;
}

const QString ChatMessageEvent::getMessageText() const {
    return mMessage;
}
