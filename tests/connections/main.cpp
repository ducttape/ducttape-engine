#include <map>
#include <memory>
#include <iostream>
#include <random>

#include "Root.hpp"
#include "network/ConnectionsManager.hpp"
#include "utils/Random.hpp"

int main() {
    bool broken = false;
    std::map<uint16_t, std::shared_ptr<dt::Connection>> connections;
    dt::ConnectionsManager connections_manager;

    uint16_t max_connections = 50;
    connections_manager.SetMaxConnections(max_connections); // counts from 1

    // Test AddConnection()
    for(uint8_t i = 0; i <= max_connections; ++i) { // this will get us up to 21 connections
        uint8_t ip = dt::Random::Get(1, 255);
        uint16_t port = dt::Random::Get(1001, 51311);

        dt::Connection* connection = new dt::Connection(sf::IpAddress("127.168.178."+dt::tostr(ip)), port+i);
        uint16_t connection_id = connections_manager.AddConnection(connection);
        if(connection_id != 0) {
           connections[connection_id] = std::shared_ptr<dt::Connection>(connection);

           // Test GetConnection()
           if(!(connections[connection_id]->GetIPAddress() == connections_manager.GetConnection(connection_id)->GetIPAddress() && \
                       connections[connection_id]->GetPort() == connections_manager.GetConnection(connection_id)->GetPort())) {
               broken = true;
               std::cerr << "ConnectionsManager: connections should be equaly." << std::endl;
           } else {
               // Test GetConnectionID()
               dt::Connection* tmp_connection = connections_manager.GetConnection(connection_id);
               if(tmp_connection != nullptr) {
                   if(connection_id != connections_manager.GetConnectionID(*(tmp_connection))) {
                       broken = true;
                       std::cerr << "ConnectionsManager: connection id should be equal." << std::endl;
                   }
               } else {
                   broken = true;
                   std::cerr << "ConnectionsManager: does not know connection id." << std::endl;
               }
           }

           if(i == max_connections) {
               broken = true;
               std::cerr << "ConnectionsManager: MaxConnections not working properly." << std::endl;
           }

        } else {
            if(i != max_connections) {
                broken = true;
                std::cerr << "ConnectionsManager: failed on adding connection." << std::endl;
            }
        }
    }

    if(broken) {
        std::cerr << "ConnectionsManager: FAIL - test canceld" << std::endl;
        return 2;
    }

    // Test IsKnownConnection()
    for(std::pair<uint16_t, std::shared_ptr<dt::Connection>> c : connections) {
        if(!connections_manager.IsKnownConnection(*(c.second.get()))) {
            broken = true;
            std::cerr << "ConnectionsManager: should know the requested connection." << std::endl;
        }
    }

    // Test RemoveConnection()
    int i = 0;
    for(std::pair<uint16_t, std::shared_ptr<dt::Connection>> c : connections) {
        if(i<=10) {
            connections_manager.RemoveConnection(c.first);
            if(connections_manager.GetConnection(c.first) != nullptr) {
                broken = true;
                std::cerr << "ConnectionsManager: no proper deletion. RemoveConnection(c.first) // by ID" << std::endl;
            }
        } else {
            connections_manager.RemoveConnection(*(c.second.get()));
            if(connections_manager.GetConnectionID(*(c.second.get())) != 0) {
                broken = true;
                std::cerr << "ConnectionsManager: no proper deletion. RemoveConnection(*(c.second.get())) // by copy" << std::endl;
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
