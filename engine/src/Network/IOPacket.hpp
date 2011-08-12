
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

namespace dt {

/**
  * Class for simplifying the code needed for serialization and deserialization of Objects into/from sf::Packets. This uses
  * the \b operator& to combine both serialization and deserialization in the same method. The data will be streamed based
  * on the context the IOPacket was created in.
  * @see sf::Packet
  * @see EnumHelper
  * @code
  * sf::Packet packet;
  * IOPacket iop(&packet, IOPacket::MODE_SEND); // for serialization or
  * // IOPacket iop(&packet, IOPacket::MODE_RECEIVE); // for deserialization
  * iop & some_variable;
  * iop & EnumHelper(my_enum); // see EnumHelper
  * // send the packet
  * @endcode
  * @note Credit for the idea of using the operator& for context based streaming goes to the authors of
  * boost serialization (boost::archive).
  */
class DUCTTAPE_API IOPacket {
public:
    /**
      * Mode for streaming data.
      */
    enum Mode {
        MODE_RECEIVE,   //!< Deserialization (operator>>)
        MODE_SEND       //!< Serialization (operator<<)
    };

    /**
      * Advanced Constructor.
      * @param packet A pointer to the sf::Packet to stream into/from.
      * @param mode Mode for streaming data (see IOPacket::Mode).
      */
    IOPacket(sf::Packet* packet = new sf::Packet(), Mode mode = MODE_RECEIVE);

    /**
      * Returns the mode set for this IOPacket.
      * @returns The mode set for this IOPacket.
      */
    Mode GetMode() const;

    /**
      * Operator& used to either stream the data into the packet or out of it. Applicable to all data types supported by
      * sf::Packet stream operators.
      * @param t Variable to stream into / from.
      * @returns This IOPacket itself to combine multiple stream instruction in one statement.
      */
    template <typename T> IOPacket& operator & (T& t) {
        if(mMode == MODE_RECEIVE)
            *mPacket >> t;
        else
            *mPacket << t;
        return *this;
    }

    /**
      * Operator& used to stream an enum using the EnumHelper class.
      * @see EnumHelper
      * @see template \<typename T> IOPacket& operator & (T& t);
      * @param h The EnumHelper instance to use for streaming.
      * @returns This IOPacket itself to combine multiple stream instruction in one statement.
      */
    IOPacket& operator & (EnumHelper h);

    /**
      * Operator& used to stream a QString.
      * @see template \<typename T> IOPacket& operator & (T& t);
      * @param s The QString object.
      * @returns This IOPacket itself to combine multiple stream instruction in one statement.
      */
    IOPacket& operator & (QString& s);

private:
    sf::Packet* mPacket;    //!< A pointer to the packet to stream data from/into.
    Mode mMode;             //!< The streaming mode.
};

}

#endif
