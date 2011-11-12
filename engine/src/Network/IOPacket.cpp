
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include <Network/IOPacket.hpp>

#include <Utils/Utils.hpp>

#include <boost/uuid/uuid_io.hpp>

namespace dt {

IOPacket::IOPacket(sf::Packet* packet, Mode mode)
    : mPacket(packet),
      mMode(mode) {}

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

IOPacket& IOPacket::operator & (QString& s) {
    if(mMode == MODE_RECEIVE) {
        std::string stdstr;
        *mPacket >> stdstr;
        s = QString(stdstr.c_str());
    } else {
        *mPacket << Utils::ToStdString(s);
    }
    return *this;
}

IOPacket& IOPacket::operator & (boost::uuids::uuid& id) {
    if(mMode == MODE_RECEIVE) {
        std::string stdstr;
        *mPacket >> stdstr;
        id = Utils::GenerateUUIDFromString(QString::fromStdString(stdstr));
    } else {
        *mPacket << boost::uuids::to_string(id);
    }
    return *this;
}

IOPacket& IOPacket::operator & (Ogre::Vector3& vector) {
    *this & vector.x;
    *this & vector.y;
    *this & vector.z;
    return *this;
}

IOPacket& IOPacket::operator & (Ogre::Quaternion& quaternion) {
    *this & quaternion.w;
    *this & quaternion.x;
    *this & quaternion.y;
    *this & quaternion.z;
    return *this;
}

}
