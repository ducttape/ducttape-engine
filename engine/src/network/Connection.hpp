#ifndef DUCTTAPE_ENGINE_NETWORK_CONNECTION
#define DUCTTAPE_ENGINE_NETWORK_CONNECTION

#include <SFML/Network.hpp>

namespace dt {

class Connection {
public:
    Connection();
    Connection(sf::IpAddress address, uint16_t port);
    ~Connection();

    void SetIPAddress(sf::IpAddress address);
    sf::IpAddress GetIPAddress() const;

    void SetPort(uint16_t port);
    uint16_t GetPort() const;

private:
    sf::IpAddress mIPAddress;
    uint16_t mPort;

};

}

#endif
