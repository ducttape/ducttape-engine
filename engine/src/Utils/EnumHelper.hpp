
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_UTILS_ENUMHELPER
#define DUCTTAPE_ENGINE_UTILS_ENUMHELPER

#include <Config.hpp>

#include <SFML/Network/Packet.hpp>

#include <cstdint>

namespace dt {

/**
  * Helper class to serialize enums into sf::Packets. This is used by the IOPacket to serialize enums even easier.
  * @code
  * enum TestEnum {
  *     VALUE_A (1),  // make sure to assign
  *     VALUE_B (2)   // some integer values here
  * };
  *
  * sf::Packet sf_packet;
  * TestEnum my_enum;
  *
  * // Serialization
  * sf_packet << EnumHelper(my_enum);
  *
  * // Deserialization
  * sf_packet >> EnumHelper(my_enum);
  * @endcode
  * @see sf::Packet
  * @see IOPacket
  */
class DUCTTAPE_API EnumHelper {
public:
    /**
      * Constructor.
      * @param e Pointer to enum.
      */
    EnumHelper(void* e);

    /**
      * Returns the integer value of the enum object.
      * @returns The integer value of the enum object.
      */
    uint32_t get();

    /**
      * Sets a new value as for the enum object.
      * @param i The new integer value.
      */
    void set(uint32_t i);

private:
    void* mEnumObj; //!< A pointer to the enum object.
};

/**
  * Stream operator to serialize the EnumHelper using it's Get() method.
  * @see EnumHelper::Get()
  * @param p The packet to stream into.
  * @param e The EnumHelper holding a pointer to the enum.
  * @returns The original sf::Packet to continue streaming.
  * @fn sf::Packet& EnumHelper::operator << (sf::Packet& p, EnumHelper e);
  */
DUCTTAPE_API sf::Packet& operator << (sf::Packet& p, EnumHelper e);

/**
  * Stream operator to deserialize the EnumHelper using it's Set() method.
  * @see EnumHelper::Get()
  * @param p The packet to stream from.
  * @param e The EnumHelper holding a pointer to the enum.
  * @returns The original sf::Packet to continue streaming.
  */
DUCTTAPE_API sf::Packet& operator >> (sf::Packet& p, EnumHelper e);

} // namespace dt

#endif
