#include "Root.hpp"

#include "network/ConnectionsManager.hpp"

#include <iostream>

int main() {

    dt::ConnectionsManager *ConnectionsManager = new dt::ConnectionsManager();

    std::cout << "ConnectionsManager: add connection" << std::endl;
    id_t c = ConnectionsManager->AddConnection(new dt::Connection(sf::IpAddress::LocalHost, (uint16_t)205001));

    std::cout << "ConnectionsManager: get ptr to previously added connection" << std::endl;
    dt::Connection *cp = ConnectionsManager->GetConnection(c);

    std::cout << "ConnectionsManager: try to get IP from ptr" << std::endl;
    sf::IpAddress cpp = cp->GetIPAddress();
    if(cpp==sf::IpAddress::LocalHost)
        std::cout << "ConnectionsManager: IP OK" << std::endl;
    else
        std::cerr << "ConnectionsManager: IP FAIL" << std::endl;

    std::cout << "ConnectionsManager: try to get Port from prt" << std::endl;
    if(cp->GetPort()==(uint16_t)205001)
        std::cout << "ConnectionsManager: Port OK" << std::endl;
    else
        std::cerr << "ConnectionsManager: Port FAIL" << std::endl;

    std::cout << "ConnectionsManager: OK" << std::endl;

    return 0;
}
