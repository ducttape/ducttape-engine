#include "IOPacket.hpp"

namespace dt {

IOPacket::IOPacket(sf::Packet* packet, Mode mode) {
    mPacket = packet;
    mMode = mode;
}

IOPacket::Mode IOPacket::GetMode() const {
    return mMode;
}

IOPacket& IOPacket::operator & (EnumHelper h) {
    if(mMode == MODE_RECEIVE)
        *mPacket >> h;
    else
        *mPacket << h;
    return *this;
}

}
