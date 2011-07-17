
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
#include "event/Timer.hpp"
#include "event/PingEvent.hpp"

namespace dt {

/**
  * Class for managing all Connections.
  * @see Connection
  */
class ConnectionsManager : public EventListener {
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
      * Initializes the manager.
      */
    void Initialize();

    /**
      * Deinitializes the manager.
      */
    void Deinitialize();

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

    /**
      * Sets the interval between two pings. Set this to 0 to disable pings. Default: 1000.
      * @param ping_interval The interval between two pings, in milliseconds.
      */
    void SetPingInterval(uint32_t ping_interval);

    /**
      * Returns the interval between two pings.
      * @return The interval between two pings, in milliseconds.
      */
    uint32_t GetPingInterval();

    /**
      * Sets the time until a connection times out. Set this to 0 to disable timeouts. Default: 10000.
      * @param timeout The time until a connection times out, in milliseconds.
      */
    void SetTimeout(uint32_t timeout);

    /**
      * Returns the time until a connection times out.
      * @returns The time until a connection times out, in milliseconds.
      */
    uint32_t GetTimeout();

    void HandleEvent(Event* e);

    /**
      * Returns the ping of a connection.
      * @param connection The ID of the connection.
      * @returns The ping of the connection.
      */
    uint32_t GetPing(ID_t connection);

private:
    /**
      * Private method. Finds an unused ID to assign to the next Connection.
      * @returns An unused ID.
      */
    ID_t _GetNewID();

    /**
      * Private method. Sends out a PingEvent.
      */
    void _Ping();

    /**
      * Private method. Handles an incoming ping event.
      * @param ping_event The ping event.
      */
    void _HandlePing(PingEvent* ping_event);

    /**
      * Private method. Checks all connections for timeouts.
      */
    void _CheckTimeouts();

    /**
      * Private method. Called when a connection times out.
      * @param connection The ID of the connection that timed out.
      */
    void _TimeoutConnection(ID_t connection);

    ID_t mMaxConnections;                           //!< The maximum number of Connections allowed.
    boost::ptr_map<ID_t, Connection> mConnections;  //!< The Connections known to this manager.
    std::map<ID_t, uint16_t> mPings;                //!< The pings for the different Connections.
    std::map<ID_t, uint16_t> mLastActivity;         //!< The time the connection sent the last packet.

    uint32_t mTimeout;      //!< The time to wait before a connection times out. In milliseconds.
    uint32_t mPingInterval; //!< The interval in milliseconds between two pings.
    Timer* mPingTimer;      //!< The timer for when to send out pings.
};

}

#endif
