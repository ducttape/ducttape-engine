
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include <Network/ConnectionsManager.hpp>

#include <Core/Root.hpp>
#include <Network/GoodbyeEvent.hpp>
#include <Network/NetworkManager.hpp>
#include <Utils/Utils.hpp>
#include <Utils/LogManager.hpp>

namespace dt {

ConnectionsManager::ConnectionsManager(ConnectionsManager::ID_t max_connections)
    : mMaxConnections(max_connections),
      mTimeout(10.0),
      mPingInterval(1.0) {}

ConnectionsManager::~ConnectionsManager() {}

void ConnectionsManager::initialize() {
    //connect((QObject*)NetworkManager::Get(), SIGNAL(NetworkManager::Get()->NewEvent(std::shared_ptr<NetworkEvent>)),
    //        this, SLOT(HandleEvent(std::shared_ptr<NetworkEvent>)));
    setPingInterval(mPingInterval); // this starts the timer
}

void ConnectionsManager::deinitialize() {
}

ConnectionsManager* ConnectionsManager::get() {
    return NetworkManager::get()->getConnectionsManager();
}

void ConnectionsManager::setMaxConnections(ConnectionsManager::ID_t max) {
    mMaxConnections = max;
}

ConnectionsManager::ID_t ConnectionsManager::getMaxConnections() const {
    return mMaxConnections;
}

bool ConnectionsManager::isKnownConnection(Connection c) {
    return getConnectionID(c) != 0;
}

ConnectionsManager::ID_t ConnectionsManager::addConnection(Connection* c) {
    ConnectionsManager::ID_t id = _getNewID();
    if(id != 0)
        mConnections[id] = Connection::ConnectionSP(c);
    return id;
}

void ConnectionsManager::removeConnection(ConnectionsManager::ID_t id) {
    if(getConnection(id) != nullptr) {
        mConnections.erase(id);
    }
}

void ConnectionsManager::removeConnection(Connection c) {
    removeConnection(getConnectionID(c));
}

ConnectionsManager::ID_t ConnectionsManager::getConnectionID(Connection c) {
    // run through connections in a loop
    for(std::map<ConnectionsManager::ID_t, Connection::ConnectionSP>::iterator i = 
                                        mConnections.begin(); i != mConnections.end(); ++i) {
        if(i->second->getPort() == c.getPort() && i->second->getIPAddress() == c.getIPAddress()) {
            return i->first;
        }
    }

    // nothing found, return 0
    return 0;
}

Connection::ConnectionSP ConnectionsManager::getConnection(ConnectionsManager::ID_t id) {
    if(mConnections.count(id) > 0)
        return mConnections.find(id)->second;
    else
        return Connection::ConnectionSP();
}

std::vector<Connection::ConnectionSP> ConnectionsManager::getAllConnections() {
    std::vector<Connection::ConnectionSP> result;

    for(std::map<ConnectionsManager::ID_t, Connection::ConnectionSP>::iterator i = 
                                mConnections.begin(); i != mConnections.end(); ++i) {
        result.push_back(i->second);
    }

    return result;
}

double ConnectionsManager::getPing(ConnectionsManager::ID_t connection) {
    return mPings[connection];
}

ConnectionsManager::ID_t ConnectionsManager::_getNewID() {
    if(mMaxConnections == 0) {
        // yeah maximum fun! sorry, we only support 16 bit IDs, so maximum connections limit
        // is at 65535
        mMaxConnections = 65535;
    }

    // run until we find an empty one
    for(ConnectionsManager::ID_t i = 1; i <= mMaxConnections; ++i) {
        if(getConnection(i) == nullptr)
            return i;
    }

    // no ID left -> all slots used
    return 0;
}

uint16_t ConnectionsManager::getConnectionCount() {
    return mConnections.size();
}

void ConnectionsManager::setPingInterval(double ping_interval) {
    mPingInterval = ping_interval;
    // reset the timer
    if(mPingTimer.get() != nullptr) {
        mPingTimer->stop();
    }
    if(mPingInterval != 0) {
        mPingTimer = std::shared_ptr<Timer>(new Timer("DT_SEND_PING", mPingInterval, true, false));
        QObject::connect((QObject*)mPingTimer.get(), SIGNAL(timerTicked(QString, double)),
                         this,                       SLOT(timerTick(QString, double)), Qt::DirectConnection);
    }
}

double ConnectionsManager::getPingInterval() {
    return mPingInterval;
}

void ConnectionsManager::setTimeout(double timeout) {
    mTimeout = timeout;
}

double ConnectionsManager::getTimeout() {
    return mTimeout;
}


void ConnectionsManager::handleEvent(std::shared_ptr<NetworkEvent> e) {
/*    if(e->GetType() == "DT_TIMERTICKEVENT") {
        std::shared_ptr<TimerTickEvent> t = std::dynamic_pointer_cast<TimerTickEvent>(e);
        if(t->GetMessageText() == "DT_SEND_PING" && t->GetInterval() == mPingInterval) {
            // this is our timer
            _Ping();
            _CheckTimeouts();
        }
    } else */ if(e->getType() == "DT_PINGEVENT") {
        std::shared_ptr<PingEvent> p = std::dynamic_pointer_cast<PingEvent>(e);
        if(p->isLocalEvent()) {
            // yes, we received this from the network
            if(p->isReply()) {
                _handlePing(p);
            } else {
                // just reply!
                // time's crucial, send directly
                std::shared_ptr<PingEvent> ping(new PingEvent(p->getTimestamp(), true));
                NetworkManager::get()->queueEvent(ping);
                NetworkManager::get()->sendQueuedEvents();
            }
        }
    }

    //if(e->IsNetworkEvent()) {
    //    std::shared_ptr<NetworkEvent> n = std::dynamic_pointer_cast<NetworkEvent>(e);
        if(e->isLocalEvent()) {
            // we received a network event
            mLastActivity[e->getSenderID()] = Root::getInstance().getTimeSinceInitialize();
        }
    //}
}

void ConnectionsManager::timerTick(QString message, double interval) {
    if(message == "DT_SEND_PING" && interval == mPingInterval) {
        // this is our timer
        _ping();
        _checkTimeouts();
    }
}


void ConnectionsManager::_ping() {
    NetworkManager::get()->queueEvent(std::make_shared<PingEvent>(Root::getInstance().getTimeSinceInitialize()));
}

void ConnectionsManager::_handlePing(std::shared_ptr<PingEvent> ping_event) {
    uint32_t ping = Root::getInstance().getTimeSinceInitialize() - ping_event->getTimestamp();
    mPings[ping_event->getSenderID()] = ping;

    Logger::get().debug("Ping for connection #" + Utils::toString(ping_event->getSenderID()) + ": " + Utils::toString(ping));
}

void ConnectionsManager::_checkTimeouts() {
    uint32_t time = Root::getInstance().getTimeSinceInitialize();
    for(auto i = mConnections.begin(); i != mConnections.end(); ++i) {
        uint32_t diff = time - mLastActivity[i->first];
        if(diff > mTimeout) {
            _timeoutConnection(i->first);
        }
    }
}

void ConnectionsManager::_timeoutConnection(ConnectionsManager::ID_t connection) {
    Logger::get().warning("Connection timed out: " + Utils::toString(connection));

    uint32_t diff = Root::getInstance().getTimeSinceInitialize() - mLastActivity[connection];

    // Send the event, hoping it will arrive at the destination
    std::shared_ptr<GoodbyeEvent> e(new GoodbyeEvent("Timeout after " + Utils::toString(diff) + " seconds."));
    e->clearRecipients();
    e->addRecipient(connection);
    // send it directly
    NetworkManager::get()->queueEvent(e);
    NetworkManager::get()->sendQueuedEvents();

    removeConnection(connection);
}

}
