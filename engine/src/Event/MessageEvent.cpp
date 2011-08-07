
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include <Event/MessageEvent.hpp>

namespace dt {

MessageEvent::MessageEvent(const QString& message)
    : mMessage(message) {}

const QString MessageEvent::GetType() const {
    return "DT_MESSAGEEVENT";
}

const QString& MessageEvent::GetMessageText() const {
    return mMessage;
}

}
