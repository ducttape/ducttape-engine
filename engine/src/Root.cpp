#include "Root.hpp"

namespace dt {

Root::Root() {
    // -- create instances of managers etc here
    mLogManager = new LogManager();
    mStringManager = new StringManager();
    mEventManager = new EventManager();
    mStateManager = new StateManager();
    mNetworkManager = new NetworkManager();
}

Root::~Root() {
    // -- delete in reverse order
    delete mNetworkManager;
    delete mStateManager;
    delete mEventManager;
    delete mStringManager;
    delete mLogManager;
}

StringManager* Root::GetStringManager() {
    return mStringManager;
}

LogManager* Root::GetLogManager() {
    return mLogManager;
}

StateManager* Root::GetStateManager() {
    return mStateManager;
}

EventManager* Root::GetEventManager() {
    return mEventManager;
}

NetworkManager* Root::GetNetworkManager() {
    return mNetworkManager;
}

}
