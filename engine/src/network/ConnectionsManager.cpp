#include "ConnectionsManager.hpp"

namespace dt {

ConnectionsManager::ConnectionsManager(ConnectionsManager::ID_t max_connections) {
    mMaxConnections = max_connections;
}

ConnectionsManager::~ConnectionsManager() {}

void ConnectionsManager::SetMaxConnections(ConnectionsManager::ID_t max) {
    mMaxConnections = max;
}

ConnectionsManager::ID_t ConnectionsManager::GetMaxConnections() const {
    return mMaxConnections;
}

bool ConnectionsManager::IsKnownConnection(Connection c) {
    return GetConnectionID(c) != 0;
}

ConnectionsManager::ID_t ConnectionsManager::AddConnection(Connection* c) {
    ConnectionsManager::ID_t id = _GetNewID();
    if(id != 0)
        mConnections[id] = *c;
    return id;
}

void ConnectionsManager::RemoveConnection(ConnectionsManager::ID_t id) {
    if(GetConnection(id) != nullptr) {
        mConnections.erase(id);
    }
}

ConnectionsManager::ID_t ConnectionsManager::GetConnectionID(Connection c) {
    // run through connections in a loop
    for(boost::ptr_map<ConnectionsManager::ID_t, Connection>::iterator i = mConnections.begin(); i != mConnections.end(); ++i) {
        if(i->second->GetPort() == c.GetPort() && i->second->GetIPAddress() == c.GetIPAddress()) {
            return i->first;
        }
    }

    // nothing found, return 0
    return 0;
}

Connection* ConnectionsManager::GetConnection(ConnectionsManager::ID_t id) {
    if(mConnections.find(id) != mConnections.end())
        return mConnections.find(id)->second;
    else
        return nullptr;
}

ConnectionsManager::ID_t ConnectionsManager::_GetNewID() {
    // run until we find an empty one
    if(mMaxConnections == 0) {
        // yeah maximum fun! sorry, we only support 16 bit IDs, so maximum connections limit
        // is at 65535
        mMaxConnections = 65535;
    }

    for(ConnectionsManager::ID_t i = 0; i < mMaxConnections; ++i) {
        if(GetConnection(i) == nullptr)
            return i;
    }

    // no ID left -> all slots used
    return 0;
}

}
