
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------


namespace dt {

Connection::Connection() {}

Connection::Connection(sf::IpAddress address, uint16_t port)
    : mIPAddress(address),
      mPort(port) {}

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
