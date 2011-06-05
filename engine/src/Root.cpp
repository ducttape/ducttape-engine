#include "Root.hpp"

namespace dt {

Root::Root() {
    // -- create instances of managers etc here
    mLogManager = new LogManager();
    mStringManager = new StringManager();
    mEventManager = new EventManager();
    mResourceManager = new ResourceManager();
    mDisplayManager = new DisplayManager();
    mComponentsManager = new ComponentsManager();
    mStateManager = new StateManager();
    mNetworkManager = new NetworkManager();
}

Root::~Root() {
    // -- delete in reverse order
    delete mNetworkManager;
    delete mStateManager;
    delete mComponentsManager;
    delete mDisplayManager;
    delete mResourceManager;
    delete mEventManager;
    delete mStringManager;
    delete mLogManager;
}

void Root::Initialize() {
    mSfClock.Reset();
    mNetworkManager->Initialize();
    mResourceManager->Initialize();
    mDisplayManager->Initialize();
    mComponentsManager->Initialize();
}

void Root::Deinitialize() {
    mComponentsManager->Deinitialize();
    mDisplayManager->Deinitialize();
    mResourceManager->Deinitialize();
    mNetworkManager->Deinitialize();
}

uint32_t Root::GetTimeSinceInitialize() {
    return mSfClock.GetElapsedTime();
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

DisplayManager* Root::GetDisplayManager() {
    return mDisplayManager;
}

ComponentsManager* Root::GetComponentsManager() {
    return mComponentsManager;
}

ResourceManager* Root::GetResourceManager() {
    return mResourceManager;
}

}
