#ifndef DUCTTAPE_ENGINE_NETWORK_NETWORKMANAGER
#define DUCTTAPE_ENGINE_NETWORK_NETWORKMANAGER

#include <boost/ptr_container/ptr_deque.hpp>
#include <boost/ptr_container/ptr_vector.hpp>

#include <SFML/Network.hpp>

#include "ConnectionsManager.hpp"
#include "event/EventListener.hpp"
#include "event/HandshakeEvent.hpp"
#include "event/GoodbyeEvent.hpp"
#include "event/NetworkEvent.hpp"
#include "utils/Utils.hpp"

namespace dt {

class NetworkManager : public EventListener {
public:
    NetworkManager();
    ~NetworkManager();

    bool BindSocket(uint16_t port = 20501);

    void Connect(Connection target);
    void Disconnect(Connection target);
    void DisconnectAll();

    void SendQueuedEvents();
    void QueueEvent(NetworkEvent* event);
    void HandleIncomingEvents();

    void HandleEvent(Event* e);

    void RegisterNetworkEventPrototype(NetworkEvent* event);
    NetworkEvent* CreatePrototypeInstance(uint32_t type_id);
    NetworkEvent* CreatePrototypeInstance(const std::string& type);

    ConnectionsManager* GetConnectionsManager();
private:
    void _SendEvent(NetworkEvent* event);

    ConnectionsManager mConnectionsManager;
    boost::ptr_deque<NetworkEvent> mQueue;

    boost::ptr_vector<NetworkEvent> mNetworkEventPrototypes;

    sf::UdpSocket mSocket;
};

}

#endif
