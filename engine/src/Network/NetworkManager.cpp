
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include <Network/NetworkManager.hpp>
#include <Network/HandshakeEvent.hpp>
#include <Network/GoodbyeEvent.hpp>

#include <Core/Root.hpp>
#include <Utils/Utils.hpp>
#include <Utils/LogManager.hpp>

namespace dt {

NetworkManager::NetworkManager() {}

NetworkManager::~NetworkManager() {}

void NetworkManager::initialize() {
    // initialize the connections mananger
    mConnectionsManager.initialize();
    QObject::connect(this,                    SIGNAL(newEvent(std::shared_ptr<dt::NetworkEvent>)),
                     getConnectionsManager(), SLOT(handleEvent(std::shared_ptr<dt::NetworkEvent>)));

    // add all default events as prototypes
    std::shared_ptr<NetworkEvent> ptr;

    ptr = std::shared_ptr<NetworkEvent>(new HandshakeEvent());
    registerNetworkEventPrototype(ptr);

    ptr = std::shared_ptr<NetworkEvent>(new GoodbyeEvent());
    registerNetworkEventPrototype(ptr);

    ptr = std::shared_ptr<NetworkEvent>(new PingEvent(0));
    registerNetworkEventPrototype(ptr);
}

void NetworkManager::deinitialize() {
    mConnectionsManager.deinitialize();
}

NetworkManager* NetworkManager::get() {
    return Root::getInstance().getNetworkManager();
}

bool NetworkManager::bindSocket(uint16_t port) {
    if(mSocket.bind(port) != sf::Socket::Done) {
        Logger::get().error("Binding socket to port " + Utils::toString(port) + " failed.");
        return false;
    }
    mSocket.setBlocking(false);
    Logger::get().info("Binding socket to port " + Utils::toString(port) + " successful.");
    return true;
}

void NetworkManager::connect(Connection target) {
    //Logger::Get().Info("New Connection : " + QString::fromStdString(target.GetIPAddress().ToString()));
    // remember target
    uint16_t id = mConnectionsManager.addConnection(&target);
    // Connection* c = mConnectionsManager.GetConnection(id);

    // send handshake
    std::shared_ptr<HandshakeEvent> h(new HandshakeEvent());
    h->clearRecipients();
    h->addRecipient(id);
    queueEvent(h);

}

void NetworkManager::disconnect(Connection target) {
    // send goodbye
    std::shared_ptr<GoodbyeEvent> goodbye(new GoodbyeEvent("Disconnected"));
    goodbye->clearRecipients();
    goodbye->addRecipient(mConnectionsManager.getConnectionID(target));
    // do not queue the event but send it directly, as we will remove the connection now
    _sendEvent(goodbye);

    mConnectionsManager.removeConnection(target);
}

void NetworkManager::disconnectAll() {
    const std::vector<Connection*>&& connections = mConnectionsManager.getAllConnections();
    for(auto iter = connections.begin(); iter != connections.end(); ++iter) {
        disconnect(**iter);
    }
}

void NetworkManager::sendQueuedEvents() {
    while(mQueue.size() > 0) {
        _sendEvent(mQueue.front());
        mQueue.pop_front();
    }
}

void NetworkManager::queueEvent(std::shared_ptr<NetworkEvent> event) {
    //Logger::Get().Debug("NetworkManager: Queued NetworkEvent [" + Utils::ToString(event->GetTypeId()) + ": " + event->GetType() + "]");
    mQueue.push_back(event);
}

void NetworkManager::handleIncomingEvents() {
    sf::Packet packet;
    sf::IpAddress remote;
    uint16_t port;
    if(mSocket.receive(packet, remote, port) == sf::Socket::Done) {
        // check if sender is known, otherwise add it
        Connection sender(remote, port);
        uint16_t sender_id = 0;
        if(mConnectionsManager.isKnownConnection(sender)) {
            sender_id = mConnectionsManager.getConnectionID(sender);
        } else {
            sender_id = mConnectionsManager.addConnection(&sender);
        }

        while(!packet.endOfPacket()) {
            uint16_t type;
            packet >> type;
            std::shared_ptr<NetworkEvent> event = createPrototypeInstance(type);
            if(event != nullptr) {
                // Logger::Get().Debug("NetworkManager: Received event [" + Utils::ToString(event->GetTypeId()) + ": " +
                                   // event->GetType() + "] from <" + Utils::ToString(sender_id) + ">. Handling.");
                IOPacket iop(&packet, IOPacket::DESERIALIZE);
                event->serialize(iop);
                event->isLocalEvent(true);
                event->setSenderID(sender_id);
                handleEvent(event);
            } else {
                Logger::get().error("NetworkManager: Cannot create instance of packet type [" + Utils::toString(type) + "]. Skipping packet.");
                break;
            }
        }
    }
}


void NetworkManager::handleEvent(std::shared_ptr<NetworkEvent> e) {
    if(!e->isLocalEvent()) {
        queueEvent(e);
        // this event was supposed to be queued and should not be handled any further
        return;
    }

    if(e->getType() == "DT_HANDSHAKEEVENT") {
        // new client connected / server replied
        std::shared_ptr<HandshakeEvent> h = std::dynamic_pointer_cast<HandshakeEvent>(e);
        if(h->getSenderID() != 0) {

        }
    } else if(e->getType() == "DT_GOODBYEEVENT") {
        // client sent a goodbye event / server will disconnect the client
        std::shared_ptr<GoodbyeEvent> g = std::dynamic_pointer_cast<GoodbyeEvent>(e);
        if(g->getSenderID() != 0) {
            mConnectionsManager.removeConnection(g->getSenderID());
        }
    }
    emit newEvent(e);
}

void NetworkManager::registerNetworkEventPrototype(std::shared_ptr<NetworkEvent> event) {
    mNetworkEventPrototypes.push_back(event);
    // register the ID (if not already happened)
    registerEvent(event->getType());
}

std::shared_ptr<NetworkEvent> NetworkManager::createPrototypeInstance(uint16_t type_id) {
    for(auto iter = mNetworkEventPrototypes.begin(); iter != mNetworkEventPrototypes.end(); ++iter) {
        if(getEventId((*iter)->getType()) == type_id) {
            return std::dynamic_pointer_cast<NetworkEvent>((*iter)->clone());
        }
    }
    return nullptr;
}

ConnectionsManager* NetworkManager::getConnectionsManager() {
    return &mConnectionsManager;
}

uint16_t NetworkManager::registerEvent(const QString name) {
    if(!eventRegistered(name)) {
        mLastEventId++;
        mEventIds[mLastEventId] = name;
        return mLastEventId;
    } else {
        Logger::get().debug("Event " + name + " already registered with id " + Utils::toString(getEventId(name)) + ".");
        return getEventId(name);
    }
}

bool NetworkManager::registerEvent(const QString name, uint16_t id) {
    if (!(eventRegistered(name) || eventRegistered(id))) {
        mEventIds[id] = name;
        return true;
    } else {
        Logger::get().debug("That Event has already been registered or that id has already been taken.");
        return false;
    }
}

bool NetworkManager::unregisterEvent(const QString name) {
    if(eventRegistered(name)) {
        mEventIds[getEventId(name)] = "";
        return true;
    } else {
        Logger::get().debug("Event " + name + " was never registered.");
        return false;
    }
}

bool NetworkManager::eventRegistered(const QString name) {
    for(auto iter = mEventIds.begin(); iter != mEventIds.end(); ++iter) {
        if(iter->second == name)
            return true;
    }
    return false;
}

bool NetworkManager::eventRegistered(uint16_t id) {
    return mEventIds.count(id) > 0 && mEventIds[id] != "";
}

uint16_t NetworkManager::getEventId(const QString name) {
    for(auto iter = mEventIds.begin(); iter != mEventIds.end(); ++iter) {
        if(iter->second == name)
            return iter->first;
    }
    return 0;
}

const QString NetworkManager::getEventString(uint16_t id) {
    return mEventIds[id];
}

void NetworkManager::_sendEvent(std::shared_ptr<NetworkEvent> event) {
    // create packet
    sf::Packet p;
    p << getEventId(event->getType());
    IOPacket packet(&p, IOPacket::SERIALIZE);
    event->serialize(packet);

    // send packet to all recipients
    const std::vector<uint16_t>& recipients = event->getRecipients();
    for(auto iter = recipients.begin(); iter != recipients.end(); ++iter) {
        Connection* r = mConnectionsManager.getConnection(*iter);
        if(r == nullptr) {
            Logger::get().error("Cannot send event to " + Utils::toString(*iter) + ": No connection with this ID");
        } else {
            mSocket.send(p, r->getIPAddress(), r->getPort());
        }
    }
}

}
