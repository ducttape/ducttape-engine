
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_TESTS_TEST_CONNECTIONS
#define DUCTTAPE_ENGINE_TESTS_TEST_CONNECTIONS

#include <Core/Root.hpp>
#include <Network/ConnectionsManager.hpp>
#include <Utils/Random.hpp>
#include <Utils/Utils.hpp>

#include <cstdint>
#include <iostream>
#include <map>
#include <memory>
#include <random>

namespace test_connections {

int Run(int argc, char** argv) {
    bool broken = false;
    std::map<uint16_t, std::shared_ptr<dt::Connection>> connections;
    dt::ConnectionsManager connections_manager;

    uint16_t max_connections = 50;
    connections_manager.SetMaxConnections(max_connections); // counts from 1

    // Test AddConnection()
    for(uint8_t i = 0; i <= max_connections; ++i) { // this will get us up to 21 connections
        uint16_t ip = dt::Random::Get(1, 255);
        uint16_t port = dt::Random::Get(1001, 51311);

        auto connection = std::shared_ptr<dt::Connection>(new dt::Connection(sf::IpAddress("127.168.178." + dt::Utils::ToString(ip).toStdString()), port+i));
        uint16_t connection_id = connections_manager.AddConnection(connection.get());
        if(connection_id != 0) {
           connections[connection_id] = std::shared_ptr<dt::Connection>(connection);

           // Test GetConnection()
           if(!(connections[connection_id]->GetIPAddress() == connections_manager.GetConnection(connection_id)->GetIPAddress() && \
                       connections[connection_id]->GetPort() == connections_manager.GetConnection(connection_id)->GetPort())) {
               broken = true;
               std::cerr << "ConnectionsManager: connections should be equal" << std::endl;
           } else {
               // Test GetConnectionID()
               dt::Connection* tmp_connection = connections_manager.GetConnection(connection_id);
               if(tmp_connection != nullptr) {
                   if(connection_id != connections_manager.GetConnectionID(*(tmp_connection))) {
                       broken = true;
                       std::cerr << "ConnectionsManager: connection ids should be equal" << std::endl;
                   }
               } else {
                   broken = true;
                   std::cerr << "ConnectionsManager: does not know connection id" << std::endl;
               }
           }

           if(i >= max_connections) {
               broken = true;
               std::cerr << "ConnectionsManager: MaxConnections not working properly" << std::endl;
           }

        } else {
            if(i != max_connections) {
                broken = true;
                std::cerr << "ConnectionsManager: failed on adding connection" << std::endl;
            }
        }
    }

    if(broken) {
        std::cerr << "ConnectionsManager: FAIL - further tests canceled" << std::endl;
        return 2;
    }

    // Test IsKnownConnection()
    for(auto iter = connections.begin(); iter != connections.end(); ++iter) {
        if(!connections_manager.IsKnownConnection(*(iter->second.get()))) {
            broken = true;
            std::cerr << "ConnectionsManager: should know the requested connection" << std::endl;
        }
    }

    // Test RemoveConnection()
    int i = 0;
    for(auto iter = connections.begin(); iter != connections.end(); ++iter) {
        if(i<=10) {
            connections_manager.RemoveConnection(iter->first);
            if(connections_manager.GetConnection(iter->first) != nullptr) {
                broken = true;
                std::cerr << "ConnectionsManager: no proper deletion using RemoveConnection(c.first) // by ID" << std::endl;
            }
        } else {
            connections_manager.RemoveConnection(*(iter->second.get()));
            if(connections_manager.GetConnectionID(*(iter->second.get())) != 0) {
                broken = true;
                std::cerr << "ConnectionsManager: no proper deletion using RemoveConnection(*(c.second.get())) // by copy" << std::endl;
            }
        }
        ++i;
    }

    if(broken) {
        std::cerr << "ConnectionsManager: FAIL" << std::endl;
        return 1;
    }
    std::cout << "ConnectionsManager: OK" << std::endl;
    return 0;
}

} // namespace test_connections

#endif
