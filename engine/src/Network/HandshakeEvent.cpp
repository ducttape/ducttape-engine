
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include <Network/HandshakeEvent.hpp>

namespace dt {

HandshakeEvent::HandshakeEvent() {}

const QString HandshakeEvent::GetType() const {
    return "DT_HANDSHAKEEVENT";
}

std::shared_ptr<Event> HandshakeEvent::Clone() const {
    std::shared_ptr<Event> ptr(new HandshakeEvent());
    return ptr;
}

void HandshakeEvent::Serialize(IOPacket& p) {
    // do nothing, we have no data
}

}
