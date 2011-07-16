
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_NETWORK_CONNECTIONSMANAGER
#define DUCTTAPE_ENGINE_NETWORK_CONNECTIONSMANAGER

#include <boost/ptr_container/ptr_map.hpp>

#include <vector>

#include "Connection.hpp"

namespace dt {

/**
  * Class for managing all Connections.
  * @see Connection
  */
class ConnectionsManager {
public:
    /**
      * Type of Connection IDs. Limits the number of maximum connections.
      */
    typedef uint16_t ID_t;

public:
    /**
      * Advanced Constructor.
      * @param max_connections The maximum number of Connections allowed.
      */
    ConnectionsManager(ID_t max_connections = 0);

    /**
      * Destructor.
      */
    ~ConnectionsManager();

    /**
      * Setter for maximum number of connections allowed. Existing connections will \b not be removed
      * if this value is lowered.
      * @param max The maximum number of connections allowed.
      */
    void SetMaxConnections(ID_t max);

    /**
      * Returns the maximum number of connections allowed.
      * @returns The maximum number of connections allowed.
      */
    ID_t GetMaxConnections() const;

    /**
      * Checks whether a connection is known by this manager.
      * @param c A Connection object to search for.
      * @returns True if the connection is known, otherwise false.
      */
    bool IsKnownConnection(Connection c);

    /**
      * Adds a Connection to this manager.
      * @param c The Connection to add.
      * @returns The ID assigned to the Connection.
      */
    ID_t AddConnection(Connection* c);

    /**
      * Removes a Connection from the manager.
      * @param id The ID of the Connection to remove.
      */
    void RemoveConnection(ID_t id);

    /**
      * Removes a Connection from the manager.
      * @param c The Connection to remove.
      */
    void RemoveConnection(Connection c);

    /**
      * Returns the ID of a Connection.
      * @param c The Connection to search for.
      * @returns The ID of the Connection or 0 if it is not known.
      */
    ID_t GetConnectionID(Connection c);

    /**
      * Returns a pointer to the Connection with the ID.
      * @param id The ID to search for.
      * @returns A pointer to the Connection or nullptr if it is not know.
      */
    Connection* GetConnection(ID_t id);

    /**
      * Returns a list of all Connections.
      * @returns A list of all Connections.
      */
    std::vector<Connection*> GetAllConnections();

    /**
     * Returns the number of active connections.
     * @returns An int of the number of active connections.
     */
    uint16_t GetConnectionCount();

private:
    /**
      * Finds an unused ID to assign to the next Connection.
      * @returns An unused ID.
      */
    ID_t _GetNewID();


    ID_t mMaxConnections;                           //!< The maximum number of Connections allowed.
    boost::ptr_map<ID_t, Connection> mConnections;  //!< The Connections known to this manager.
};

}

#endif
