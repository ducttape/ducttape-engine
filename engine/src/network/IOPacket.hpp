#ifndef DUCTTAPE_ENGINE_NETWORK_IOPACKET
#define DUCTTAPE_ENGINE_NETWORK_IOPACKET

#include <SFML/Network.hpp>
#include "utils/EnumHelper.hpp"

namespace dt {

class IOPacket {
public:
    enum Mode {
        MODE_RECEIVE,   // uses operator >>
        MODE_SEND       // uses operator <<
    };

    IOPacket(sf::Packet* packet = new sf::Packet(), Mode mode = MODE_RECEIVE);

    Mode GetMode() const;

    template <typename T> IOPacket& operator & (T& t) {
        if(mMode == MODE_RECEIVE)
            *mPacket >> t;
        else
            *mPacket << t;
        return *this;
    }

    IOPacket& operator & (EnumHelper h);
private:
    sf::Packet* mPacket;
    Mode mMode;
};

}

#endif
