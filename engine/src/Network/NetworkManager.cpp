
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include <Network/NetworkManager.hpp>

#include <Core/Root.hpp>
#include <Event/EventManager.hpp>
#include <Network/HandshakeEvent.hpp>
#include <Network/GoodbyeEvent.hpp>
#include <Utils/Utils.hpp>
#include <Utils/LogManager.hpp>
#include <Core/StringManager.hpp>

namespace dt {

NetworkManager::NetworkManager() {}

NetworkManager::~NetworkManager() {}

void NetworkManager::Initialize() {
    EventManager::Get()->AddListener(this);

    // add all default events as prototypes
    std::shared_ptr<NetworkEvent> ptr;

    ptr = std::shared_ptr<NetworkEvent>(new HandshakeEvent());
    RegisterNetworkEventPrototype(ptr);

    ptr = std::shared_ptr<NetworkEvent>(new GoodbyeEvent());
    RegisterNetworkEventPrototype(ptr);

    ptr = std::shared_ptr<NetworkEvent>(new PingEvent(0));
    RegisterNetworkEventPrototype(ptr);

    // initialize the connections mananger
    mConnectionsManager.Initialize();
}

void NetworkManager::Deinitialize() {
    mConnectionsManager.Deinitialize();
    EventManager::Get()->RemoveListener(this);
}

NetworkManager* NetworkManager::Get() {
    return Root::GetInstance().GetNetworkManager();
}

bool NetworkManager::BindSocket(uint16_t port) {
    if(mSocket.Bind(port) != sf::Socket::Done) {
        Logger::Get().Error("Binding socket to port " + Utils::ToString(port) + " failed.");
        return false;
    }
    mSocket.SetBlocking(false);
    Logger::Get().Info("Binding socket to port " + Utils::ToString(port) + " successful.");
    return true;
}

void NetworkManager::Connect(Connection target) {
    // remember target
    uint16_t id = mConnectionsManager.AddConnection(&target);
    // Connection* c = mConnectionsManager.GetConnection(id);

    // send handshake
    std::shared_ptr<HandshakeEvent> h(new HandshakeEvent());
    h->ClearRecipients();
    h->AddRecipient(id);
    QueueEvent(h);
}

void NetworkManager::Disconnect(Connection target) {
    // send goodbye
    std::shared_ptr<GoodbyeEvent> goodbye(new GoodbyeEvent("Disconnected"));
    goodbye->ClearRecipients();
    goodbye->AddRecipient(mConnectionsManager.GetConnectionID(target));
    // do not queue the event but send it directly, as we will remove the connection now
    _SendEvent(goodbye);

    mConnectionsManager.RemoveConnection(target);
}

void NetworkManager::DisconnectAll() {
    const std::vector<Connection*>&& connections = mConnectionsManager.GetAllConnections();
    for(auto iter = connections.begin(); iter != connections.end(); ++iter) {
        Disconnect(**iter);
    }
}

void NetworkManager::SendQueuedEvents() {
    while(mQueue.size() > 0) {
        _SendEvent(mQueue.front());
        mQueue.pop_front();
    }
}

void NetworkManager::QueueEvent(std::shared_ptr<NetworkEvent> event) {
    //Logger::Get().Debug("NetworkManager: Queued NetworkEvent [" + Utils::ToString(event->GetTypeID()) + ": " + event->GetType() + "]");
    mQueue.push_back(event);
}

void NetworkManager::HandleIncomingEvents() {
    sf::Packet packet;
    sf::IpAddress remote;
    uint16_t port;
    if(mSocket.Receive(packet, remote, port) == sf::Socket::Done) {
        // check if sender is known, otherwise add it
        Connection sender(remote, port);
        uint16_t sender_id = 0;
        if(mConnectionsManager.IsKnownConnection(sender)) {
            sender_id = mConnectionsManager.GetConnectionID(sender);
        } else {
            sender_id = mConnectionsManager.AddConnection(&sender);
        }

        while(!packet.EndOfPacket()) {
            uint32_t type;
            packet >> type;
            std::shared_ptr<NetworkEvent> event = CreatePrototypeInstance(type);
            if(event != nullptr) {
                // Logger::Get().Debug("NetworkManager: Received event [" + Utils::ToString(event->GetTypeID()) + ": " +
                                   // event->GetType() + "] from <" + Utils::ToString(sender_id) + ">. Handling.");
                IOPacket iop(&packet, IOPacket::MODE_RECEIVE);
                event->Serialize(iop);
                event->IsLocalEvent(true);
                event->SetSenderID(sender_id);
                EventManager::Get()->InjectEvent(event);
            } else {
                Logger::Get().Error("NetworkManager: Cannot create instance of packet type [" + Utils::ToString(type) + "]. Skipping packet.");
                break;
            }
        }
    }
}

void NetworkManager::HandleEvent(std::shared_ptr<Event> e) {
    if(e->IsNetworkEvent()) {
        std::shared_ptr<NetworkEvent> n = \
            std::dynamic_pointer_cast<NetworkEvent>(e);
        if(!n->IsLocalEvent()) {
            QueueEvent(n);
            // this event was supposed to be queued and should not be handled any further
            e->Cancel();
            return;
        }
    }

    if(e->GetType() == "DT_HANDSHAKEEVENT") {
        // new client connected / server replied
        std::shared_ptr<HandshakeEvent> h = \
            std::dynamic_pointer_cast<HandshakeEvent>(e);
        if(h->GetSenderID() != 0) {

        }
    } else if(e->GetType() == "DT_GOODBYEEVENT") {
        // client sent a godbye event / server will disconnect the client
        std::shared_ptr<GoodbyeEvent> g = \
            std::dynamic_pointer_cast<GoodbyeEvent>(e);
        if(g->GetSenderID() != 0) {
            mConnectionsManager.RemoveConnection(g->GetSenderID());
        }
    }
}

EventListener::Priority NetworkManager::GetEventPriority() const {
    return EventListener::INTERNAL_HIGHEST;
}

void NetworkManager::RegisterNetworkEventPrototype(std::shared_ptr<NetworkEvent> event) {
    mNetworkEventPrototypes.push_back(event);
    // register the ID (if not already happened)
    StringManager::Get()->Add(event->GetType());
}

std::shared_ptr<NetworkEvent> NetworkManager::CreatePrototypeInstance(uint32_t type_id) {
    for(auto iter = mNetworkEventPrototypes.begin(); iter != mNetworkEventPrototypes.end(); ++iter) {
        if((*iter)->GetTypeID() == type_id) {
            return std::dynamic_pointer_cast<NetworkEvent>((*iter)->Clone());
        }
    }
    return nullptr;
}

ConnectionsManager* NetworkManager::GetConnectionsManager() {
    return &mConnectionsManager;
}

void NetworkManager::_SendEvent(std::shared_ptr<NetworkEvent> event) {
    // create packet
    sf::Packet p;
    p << event->GetTypeID();
    IOPacket packet(&p, IOPacket::MODE_SEND);
    event->Serialize(packet);

    // send packet to all recipients
    const std::vector<uint16_t>& recipients = event->GetRecipients();
    for(auto iter = recipients.begin(); iter != recipients.end(); ++iter) {
        // Logger::Get().Debug("NetworkManager: Sending Event to " + Utils::ToString(i));
        Connection* r = mConnectionsManager.GetConnection(*iter);
        if(r == nullptr) {
            Logger::Get().Error("Cannot send event to " + Utils::ToString(*iter) + ": No connection with this ID");
        } else {
            mSocket.Send(p, r->GetIPAddress(), r->GetPort());
        }
    }
}

}
