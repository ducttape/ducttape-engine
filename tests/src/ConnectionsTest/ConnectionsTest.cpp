
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include "ConnectionsTest/ConnectionsTest.hpp"

namespace ConnectionsTest {

bool ConnectionsTest::Run(int argc, char** argv) {
    std::map<uint16_t, std::shared_ptr<dt::Connection>> connections;
    dt::ConnectionsManager connections_manager;

    uint16_t max_connections = 50;
    connections_manager.SetMaxConnections(max_connections); // counts from 1

    // Test AddConnection()
    for(uint8_t i = 0; i <= max_connections; ++i) { // this will get us up to 21 connections
        uint16_t ip = dt::Random::Get(1, 255);
        uint16_t port = dt::Random::Get(1001, 51311);

        auto connection = std::shared_ptr<dt::Connection>(new dt::Connection(sf::IpAddress("127.168.178." + dt::Utils::ToStdString(dt::Utils::ToString(ip))), port+i));
        uint16_t connection_id = connections_manager.AddConnection(connection.get());
        if(connection_id != 0) {
           connections[connection_id] = std::shared_ptr<dt::Connection>(connection);

           // Test GetConnection()
           if(!(connections[connection_id]->GetIPAddress() == connections_manager.GetConnection(connection_id)->GetIPAddress() && \
                       connections[connection_id]->GetPort() == connections_manager.GetConnection(connection_id)->GetPort())) {
               std::cerr << "Connections should be equal." << std::endl;
               return false;
           } else {
               // Test GetConnectionID()
               dt::Connection* tmp_connection = connections_manager.GetConnection(connection_id);
               if(tmp_connection != nullptr) {
                   if(connection_id != connections_manager.GetConnectionID(*(tmp_connection))) {
                       std::cerr << "Connection IDs should be equal." << std::endl;
                       return false;
                   }
               } else {
                   std::cerr << "ConnectionsManager returned unknown connection ID." << std::endl;
                   return false;
               }
           }

           if(i >= max_connections) {
               std::cerr << "ConnectionsManager did allow too many connections." << std::endl;
               return false;
           }

        } else {
            if(i != max_connections) {
                std::cerr << "ConnectionsManager failed to add a connection." << std::endl;
                return false;
            }
        }
    }

    // Test IsKnownConnection()
    for(auto iter = connections.begin(); iter != connections.end(); ++iter) {
        if(!connections_manager.IsKnownConnection(*(iter->second.get()))) {
            std::cerr << "ConnectionsManager should know the requested connection." << std::endl;
            return false;
        }
    }

    // Test RemoveConnection()
    int i = 0;
    for(auto iter = connections.begin(); iter != connections.end(); ++iter) {
        if(i<=10) {
            connections_manager.RemoveConnection(iter->first);
            if(connections_manager.GetConnection(iter->first) != nullptr) {
                std::cerr << "ConnectionsManager did not delete correctly using the ID." << std::endl;
                return false;
            }
        } else {
            connections_manager.RemoveConnection(*(iter->second.get()));
            if(connections_manager.GetConnectionID(*(iter->second.get())) != 0) {
                std::cerr << "ConnectionsManager did not delete correctly using the connection instance." << std::endl;
                return false;
            }
        }
        ++i;
    }

    return true;
}


QString ConnectionsTest::GetTestName() {
    return "Connections";
}

}
