#include "Root.hpp"

namespace dt {

Root::Root() {
    // This list of new keywords exists to allow us fine-grained control over
    // the creation and deletion of these managers.
    mLogManager = new LogManager();
    mStringManager = new StringManager();
    mEventManager = new EventManager();
    mResourceManager = new ResourceManager();
    mDisplayManager = new DisplayManager();
    mStateManager = new StateManager();
    mNetworkManager = new NetworkManager();
}

Root::~Root() {
    // Complementary to the constructor, we destroy the managers in reverse
    // order.
    delete mNetworkManager;
    delete mStateManager;
    delete mDisplayManager;
    delete mResourceManager;
    delete mEventManager;
    delete mStringManager;
    delete mLogManager;
}

void Root::Initialize(int argc, char** argv) {
    mSfClock.Reset();

    mExecutablePath = boost::filesystem::system_complete(boost::filesystem::path( argv[0]));

    mNetworkManager->Initialize();
    mResourceManager->Initialize();
    mDisplayManager->Initialize();
}

void Root::Deinitialize() {
    mDisplayManager->Deinitialize();
    mResourceManager->Deinitialize();
    mNetworkManager->Deinitialize();
}

uint32_t Root::GetTimeSinceInitialize() const {
    return mSfClock.GetElapsedTime();
}

const boost::filesystem::path& Root::GetExecutablePath() const {
    return mExecutablePath;
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

ResourceManager* Root::GetResourceManager() {
    return mResourceManager;
}

}
