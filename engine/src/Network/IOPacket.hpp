
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_NETWORK_IOPACKET
#define DUCTTAPE_ENGINE_NETWORK_IOPACKET

#include <Config.hpp>

#include <Utils/EnumHelper.hpp>

#include <QString>

#include <SFML/Network/Packet.hpp>

#include <boost/uuid/uuid.hpp>

#include <yaml-cpp/yaml.h>

#include <OgreVector3.h>
#include <OgreQuaternion.h>

// first, declare some streaming operators

namespace YAML {

const Node& operator >> (const Node& node, Ogre::Vector3& v);
Emitter& operator << (Emitter& emitter, Ogre::Vector3& v);

const Node& operator >> (const Node& node, Ogre::Quaternion& q);
Emitter& operator << (Emitter& emitter, Ogre::Quaternion& q);

} // namespace YAML

namespace dt {

sf::Packet& operator >> (sf::Packet& packet, Ogre::Vector3& v);
sf::Packet& operator << (sf::Packet& packet, Ogre::Vector3& v);

sf::Packet& operator >> (sf::Packet& packet, Ogre::Quaternion& q);
sf::Packet& operator << (sf::Packet& packet, Ogre::Quaternion& q);

/**
  * Class for simplifying the code needed for serialization and deserialization of Objects. This uses
  * the method \b Stream to combine both serialization and deserialization in the same method. The data will
  * be read or written based on the context the IOPacket was created in.
  * @see EnumHelper
  * @code
  * iopacket.Stream(some_variable, "key", default_value);
  * iopacket.Stream(EnumHelper(my_enum), "key", (uint32_t)MyEnum::DEFAULT_VALUE); // see EnumHelper
  * @endcode
  * @note Credit for the idea of context based streaming goes to the authors of
  * boost serialization (boost::archive).
  */
class DUCTTAPE_API IOPacket {
public:
    /**
      * Direction for streaming data.
      */
    enum Direction {
        DESERIALIZE,    //!< Deserialization (operator>>)
        SERIALIZE       //!< Serialization (operator<<)
    };

    enum Mode {
        BINARY, //!< sf::Packet
        TEXT    //!< Yaml
    };

    IOPacket(sf::Packet* packet, Direction direction = DESERIALIZE);
    IOPacket(YAML::Node* node);
    IOPacket(YAML::Emitter* emitter);

    /**
      * Returns the mode set for this IOPacket.
      * @returns The mode set for this IOPacket.
      */
    Mode GetMode() const;

    /**
      * Returns the mode set for this IOPacket.
      * @returns The mode set for this IOPacket.
      */
    Direction GetDirection() const;

    template <typename T>
    IOPacket& Stream(T& t, QString key, T def = T()) {
        if(mMode == BINARY) {
            if(mDirection == DESERIALIZE)
                *mPacket >> t;
            else
                *mPacket << t;
        } else {
            if(mDirection == DESERIALIZE) {
                try {
                    (*mNode)[key.toStdString()] >> t;
                } catch(YAML::TypedKeyNotFound<std::string>& e) {
                    t = def;
                }
            } else {
                *mEmitter << YAML::Key << key.toStdString() << YAML::Value << YAML::DoubleQuoted << t << YAML::Auto;
            }
        }
        return *this;
    }

    // TODO: try template specialization

    virtual IOPacket& Stream(EnumHelper h, QString key = "", uint32_t def = 0);

    virtual IOPacket& Stream(QString& s, QString key = "", QString def = "");

    virtual IOPacket& Stream(boost::uuids::uuid& id, QString key = "", boost::uuids::uuid def = boost::uuids::uuid());

    uint32_t BeginList(uint32_t count, QString key);

    void EndList();

    void BeginObject();

    void EndObject();

protected:
    Direction mDirection;   //!< The streaming direction.
    Mode mMode;             //!< The data mode.

    sf::Packet* mPacket;
    const YAML::Node* mNode;
    std::vector<const YAML::Node*> mNodeStack;
    int mIndexInSequence;
    std::vector<int> mIndexInSequenceStack;
    YAML::Emitter* mEmitter;

};

}

#endif
