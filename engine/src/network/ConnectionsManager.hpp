#ifndef DUCTTAPE_ENGINE_NETWORK_CONNECTIONSMANAGER
#define DUCTTAPE_ENGINE_NETWORK_CONNECTIONSMANAGER

#include <boost/ptr_container/ptr_map.hpp>

#include <vector>

#include "Connection.hpp"

namespace dt {

class ConnectionsManager {
public:
    typedef uint16_t ID_t;

public:
    ConnectionsManager(ID_t max_connections = 0);
    ~ConnectionsManager();

    void SetMaxConnections(ID_t max);

    ID_t GetMaxConnections() const;

    bool IsKnownConnection(Connection c);

    ID_t AddConnection(Connection* c);

    void RemoveConnection(ID_t id);

    void RemoveConnection(Connection c);

    ID_t GetConnectionID(Connection c);

    Connection* GetConnection(ID_t id);

    std::vector<Connection*> GetAllConnections();

private:
    ID_t _GetNewID();
    ID_t mMaxConnections;

    boost::ptr_map<ID_t, Connection> mConnections;
};

}

#endif
