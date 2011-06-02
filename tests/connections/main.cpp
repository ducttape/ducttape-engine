#include "Root.hpp"

#include "network/ConnectionsManager.hpp"

#include <iostream>

int main() {
    bool broken = false;
    dt::ConnectionsManager connections_manager;

    std::cout << "ConnectionsManager: add connection" << std::endl;
    uint16_t connection = connections_manager.AddConnection(new dt::Connection(sf::IpAddress::LocalHost, 51311));

    std::cout << "ConnectionsManager: get pointer to previously added connection" << std::endl;
    dt::Connection* connection_ptr = connections_manager.GetConnection(connection);

    std::cout << "ConnectionsManager: try to get IP from pointer" << std::endl;
    sf::IpAddress connection_ip = connection_ptr->GetIPAddress();
    if(connection_ip == sf::IpAddress::LocalHost) {
        std::cout << "ConnectionsManager: IP OK" << std::endl;
    } else {
        std::cerr << "ConnectionsManager: IP FAIL" << std::endl;
        broken = true;
    }

    std::cout << "ConnectionsManager: try to get Port from pointer" << std::endl;
    if(connection_ptr->GetPort() == 51311) {
        std::cout << "ConnectionsManager: Port OK" << std::endl;
    } else {
        std::cerr << "ConnectionsManager: Port FAIL" << std::endl;
        broken = true;
    }

    if(broken) {
        std::cout << "ConnectionsManager: FAIL" << std::endl;
        return 1;
    }

    std::cout << "ConnectionsManager: OK" << std::endl;
    return 0;
}
