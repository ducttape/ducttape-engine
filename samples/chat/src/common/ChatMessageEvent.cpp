
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include "ChatMessageEvent.hpp"

ChatMessageEvent::ChatMessageEvent(const QString& message, const QString& sender) :
    MessageEvent(message) {
    mSenderNick = sender;
}

const QString ChatMessageEvent::GetType() const {
    return "CHATMESSAGEEVENT";
}

std::shared_ptr<dt::Event> ChatMessageEvent::Clone() const {
    std::shared_ptr<dt::Event> ptr(new ChatMessageEvent(mMessage, mSenderNick));
    return ptr;
}

void ChatMessageEvent::Serialize(dt::IOPacket& p) {
    p & mMessage;
    p & mSenderNick;
}

const QString& ChatMessageEvent::GetSenderNick() const {
    return mSenderNick;
}
