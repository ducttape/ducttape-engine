#ifndef DUCTTAPE_ENGINE_NETWORK_NETWORKMANAGER
#define DUCTTAPE_ENGINE_NETWORK_NETWORKMANAGER

#include "event/NetworkEvent.hpp"
#include "ConnectionsManager.hpp"

namespace dt {

class NetworkManager {
public:
    NetworkManager();
    ~NetworkManager();

    void SendQueuedEvents();
    void QueueEvent(NetworkEvent* event);

private:
    ConnectionsManager mConnectionsManager;
};

}

#endif
