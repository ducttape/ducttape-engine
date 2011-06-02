#include "NetworkManager.hpp"

#include "Root.hpp"

namespace dt {

NetworkManager::NetworkManager() {}

NetworkManager::~NetworkManager() {}

void NetworkManager::Initialize() {
    Root::get_mutable_instance().GetEventManager()->AddListener(this);
}

void NetworkManager::Deinitialize() {
    Root::get_mutable_instance().GetEventManager()->RemoveListener(this);
}


bool NetworkManager::BindSocket(uint16_t port) {
    if(mSocket.Bind(port) != sf::Socket::Done) {
        Logger::Get().Error("Binding socket to port " + tostr(port) + " failed.");
        return false;
    }
    mSocket.SetBlocking(false);
    Logger::Get().Info("Binding socket to port " + tostr(port) + " successful.");
    return true;
}

void NetworkManager::Connect(Connection target) {
    // remember target
    uint16_t id = mConnectionsManager.AddConnection(&target);
    // Connection* c = mConnectionsManager.GetConnection(id);

    // send handshake
    HandshakeEvent* h = new HandshakeEvent();
    h->ClearRecipients();
    h->AddRecipient(id);
    QueueEvent(h);
}

void NetworkManager::Disconnect(Connection target) {
    // send goodbye
    GoodbyeEvent* goodbye = new GoodbyeEvent("Disconnected");
    goodbye->ClearRecipients();
    goodbye->AddRecipient(mConnectionsManager.GetConnectionID(target));
    // do not queue the event but send it directly, as we will remove the connection now
    _SendEvent(goodbye);

    mConnectionsManager.RemoveConnection(target);
}

void NetworkManager::DisconnectAll() {
    for(Connection* c: mConnectionsManager.GetAllConnections()) {
        Disconnect(*c);
    }
}

void NetworkManager::SendQueuedEvents() {
    for(auto iter = mQueue.begin(); iter != mQueue.end(); ++iter) {
        _SendEvent(&(*iter));
    }
    // clear queue after all packages have been sent
    mQueue.clear();
}

void NetworkManager::QueueEvent(NetworkEvent* event) {
    mQueue.push_back(event);
}

void NetworkManager::HandleIncomingEvents() {
    sf::Packet packet;
    sf::IpAddress remote;
    uint16_t port;
    if(mSocket.Receive(packet, remote, port) == sf::Socket::Done) {
        // handle packet
    }
}

void NetworkManager::HandleEvent(Event* e) {
    if(e->IsNetworkEvent()) {
        QueueEvent((NetworkEvent*)e);
    }
}

void NetworkManager::RegisterNetworkEventPrototype(NetworkEvent* event) {
    mNetworkEventPrototypes.push_back(event);
}

NetworkEvent* NetworkManager::CreatePrototypeInstance(uint32_t type_id) {
    for(boost::ptr_vector<NetworkEvent>::iterator iter = mNetworkEventPrototypes.begin(); iter != mNetworkEventPrototypes.end(); ++iter) {
        if(iter->GetTypeID() == type_id) {
            return iter->NewInstance();
        }
    }
    return nullptr;
}

NetworkEvent* NetworkManager::CreatePrototypeInstance(const std::string& type) {
    return CreatePrototypeInstance(Root::get_mutable_instance().GetStringManager()->Get(type));
}

ConnectionsManager* NetworkManager::GetConnectionsManager() {
    return &mConnectionsManager;
}

void NetworkManager::_SendEvent(NetworkEvent* event) {
    // create packet
    sf::Packet p;
    IOPacket packet(&p, IOPacket::MODE_SEND);
    event->Serialize(packet);

    // send packet to all recipients
    for(int i: event->GetRecipients()) {
        Connection* r = mConnectionsManager.GetConnection(i);
        mSocket.Send(p, r->GetIPAddress(), r->GetPort());
    }
}

}
