
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
    const std::vector<Connection*>&& connections = ConnectionsManager::Get()->GetAllConnections();
    for(auto iter = connections.begin(); iter != connections.end(); ++iter) {
        AddRecipient(ConnectionsManager::Get()->GetConnectionID(**iter));
    }
}

uint32_t NetworkEvent::GetTypeId() const{
    return NetworkManager::Get()->GetEventId(GetType());
}

bool NetworkEvent::IsNetworkEvent() const {
    return true;
}

void NetworkEvent::AddRecipient(uint16_t id) {
    mRecipients.push_back(id);
}

void NetworkEvent::RemoveRecipient(uint16_t id) {
    for(auto iter = mRecipients.begin(); iter != mRecipients.end(); ++iter) {
        if(*iter == id) {
            iter = mRecipients.erase(iter);
        }
    }
}

void NetworkEvent::ClearRecipients() {
    mRecipients.clear();
}

const std::vector<uint16_t>& NetworkEvent::GetRecipients() const {
    return mRecipients;
}

bool NetworkEvent::HasRecipient(uint16_t id) {
    for(auto iter = mRecipients.begin(); iter != mRecipients.end(); ++iter) {
        if(*iter == id)
            return true;
    }
    return false;
}

bool NetworkEvent::IsLocalEvent() const {
    return mIsLocalEvent;
}

void NetworkEvent::IsLocalEvent(bool is_local_event) {
    mIsLocalEvent = is_local_event;
}

uint16_t NetworkEvent::GetSenderID() const {
    return mSenderID;
}

void NetworkEvent::SetSenderID(uint16_t id) {
    mSenderID = id;
}

}
