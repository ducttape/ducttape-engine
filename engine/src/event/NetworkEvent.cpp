
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifdef COMPILER_MSVC
#include <boost/foreach.hpp>
#endif

#include "NetworkEvent.hpp"

#include "Root.hpp"

namespace dt {

NetworkEvent::NetworkEvent()
    : mSenderID(false),
     mIsLocalEvent(false) {

    // add default recipients
#ifdef COMPILER_MSVC
    BOOST_FOREACH(Connection* c, ConnectionsManager::Get()->GetAllConnections()) {
#else
    for(Connection* c: ConnectionsManager::Get()->GetAllConnections()) {
#endif
        AddRecipient(ConnectionsManager::Get()->GetConnectionID(*c));
    }
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
#ifdef COMPILER_MSVC
    BOOST_FOREACH(uint16_t i, mRecipients)
#else
    for(uint16_t i: mRecipients)
#endif
        if(i == id)
            return true;
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
