#include "Connection.hpp"

namespace dt {

Connection::Connection() {}

Connection::Connection(sf::IpAddress address, uint16_t port) {
    mIPAddress = address;
    mPort = port;
}

Connection::~Connection() {}

void Connection::SetIPAddress(sf::IpAddress address) {
    mIPAddress = address;
}

sf::IpAddress Connection::GetIPAddress() const {
    return mIPAddress;
}

void Connection::SetPort(uint16_t port) {
    mPort = port;
}

uint16_t Connection::GetPort() const {
    return mPort;
}

}
