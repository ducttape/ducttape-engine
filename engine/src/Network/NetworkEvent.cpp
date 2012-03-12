
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include <Network/NetworkEvent.hpp>

#include <Network/Connection.hpp>
#include <Network/ConnectionsManager.hpp>
#include <Network/NetworkManager.hpp>

namespace dt {

NetworkEvent::NetworkEvent()
    : mSenderID(false),
     mIsLocalEvent(false) {

    // add default recipients
    const std::vector<Connection*>&& connections = ConnectionsManager::get()->getAllConnections();
    for(auto iter = connections.begin(); iter != connections.end(); ++iter) {
        addRecipient(ConnectionsManager::get()->getConnectionID(**iter));
    }
}

uint16_t NetworkEvent::getTypeId() const{
    return NetworkManager::get()->getEventId(getType());
}

bool NetworkEvent::isNetworkEvent() const {
    return true;
}

void NetworkEvent::addRecipient(uint16_t id) {
    mRecipients.push_back(id);
}

void NetworkEvent::removeRecipient(uint16_t id) {
    for(auto iter = mRecipients.begin(); iter != mRecipients.end(); ++iter) {
        if(*iter == id) {
            iter = mRecipients.erase(iter);
        }
    }
}

void NetworkEvent::clearRecipients() {
    mRecipients.clear();
}

const std::vector<uint16_t>& NetworkEvent::getRecipients() const {
    return mRecipients;
}

bool NetworkEvent::hasRecipient(uint16_t id) {
    for(auto iter = mRecipients.begin(); iter != mRecipients.end(); ++iter) {
        if(*iter == id)
            return true;
    }
    return false;
}

bool NetworkEvent::isLocalEvent() const {
    return mIsLocalEvent;
}

void NetworkEvent::isLocalEvent(bool is_local_event) {
    mIsLocalEvent = is_local_event;
}

uint16_t NetworkEvent::getSenderID() const {
    return mSenderID;
}

void NetworkEvent::setSenderID(uint16_t id) {
    mSenderID = id;
}

}
