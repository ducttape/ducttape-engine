
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

IOPacket::IOPacket(sf::Packet* packet, Direction direction)
    : mDirection(direction),
      mMode(BINARY),
      mPacket(packet) {}

IOPacket::IOPacket(YAML::Node* node)
    : mDirection(DESERIALIZE),
      mMode(TEXT),
      mNode(node) {}

IOPacket::IOPacket(YAML::Emitter* emitter)
    : mDirection(SERIALIZE),
      mMode(TEXT),
      mEmitter(emitter) {}

IOPacket::Mode IOPacket::GetMode() const {
    return mMode;
}

IOPacket::Direction IOPacket::GetDirection() const {
    return mDirection;
}

IOPacket& IOPacket::Stream(EnumHelper h, QString key, uint32_t def) {
    if(mDirection == DESERIALIZE) {
        uint32_t x = 0;
        Stream(x, key, def);
        h.Set(x);
    }
    else {
        uint32_t x = h.Get();
        Stream(x, key, def);
    }
    return *this;
}

IOPacket& IOPacket::Stream(QString& s, QString key, QString def) {
    if(mDirection == DESERIALIZE) {
        std::string stdstr;
        Stream(stdstr, key, def.toStdString());
        s = QString(stdstr.c_str());
    } else {
        std::string stdstr = Utils::ToStdString(s);
        Stream(stdstr, key, def.toStdString());
    }
    return *this;
}

IOPacket& IOPacket::Stream(boost::uuids::uuid& id, QString key, boost::uuids::uuid def) {
    if(mDirection == DESERIALIZE) {
        std::string stdstr;
        Stream(stdstr, key);
        if(stdstr == "") id = def;
        else id = Utils::GenerateUUIDFromString(QString::fromStdString(stdstr));
    } else {
        std::string stdstr = boost::uuids::to_string(id);
        Stream(stdstr, key);
    }
    return *this;
}

IOPacket& IOPacket::Stream(Ogre::Vector3& vector, QString key, Ogre::Vector3 def) {
    Stream(vector.x, key + ".x", def.x);
    Stream(vector.y, key + ".y", def.y);
    Stream(vector.z, key + ".z", def.z);
    return *this;
}

IOPacket& IOPacket::Stream(Ogre::Quaternion& quaternion, QString key, Ogre::Quaternion def) {
    Stream(quaternion.w, key + ".w", def.w);
    Stream(quaternion.x, key + ".x", def.x);
    Stream(quaternion.y, key + ".y", def.y);
    Stream(quaternion.z, key + ".z", def.z);
    return *this;
}

uint32_t IOPacket::BeginList(uint32_t count, QString key) {
    if(mMode == BINARY) {
        Stream(count, "count"); // Notice: key does not matter in binary mode
    } else {
        mIndexInSequenceStack.push_back(mIndexInSequence);
        mIndexInSequence = 0;
        if(mDirection == SERIALIZE) {
            *mEmitter << YAML::Key << key.toStdString();
            *mEmitter << YAML::Value << YAML::BeginSeq;
        } else {
            mNodeStack.push_back(mNode);
            mNode = &((*mNode)[key.toStdString()]);
            return mNode->size();
        }
    }
    return count;
}

void IOPacket::EndList() {
    if(mMode == TEXT) {
        if(mIndexInSequenceStack.size() > 0) {
            mIndexInSequence = mIndexInSequenceStack.back();
            mIndexInSequenceStack.pop_back();
        } else {
            mIndexInSequence = 0;
        }

        if(mDirection == SERIALIZE) {
            *mEmitter << YAML::EndSeq;
        } else {
            mNode = mNodeStack.back();
            mNodeStack.pop_back();
        }
    }
}

void IOPacket::BeginObject() {
    if(mMode == TEXT) {
        if(mDirection == SERIALIZE) {
            *mEmitter << YAML::BeginMap;
        } else {
            mNodeStack.push_back(mNode);
            mNode = &((*mNode)[mIndexInSequence]);
        }
    }
}

void IOPacket::EndObject() {
    if(mMode == TEXT) {
        if(mDirection == SERIALIZE) {
            *mEmitter << YAML::EndMap;
        } else {
            mNode = mNodeStack.back();
            mNodeStack.pop_back();
            ++mIndexInSequence;
        }
    }
}

}
