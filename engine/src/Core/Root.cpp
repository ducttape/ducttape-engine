
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include <Core/Root.hpp>

#include <Utils/LogManager.hpp>
#include <Core/ResourceManager.hpp>
#include <Input/InputManager.hpp>
#include <Graphics/DisplayManager.hpp>
#include <Scene/StateManager.hpp>
#include <Scene/Serializer.hpp>
#include <Logic/NavigationManager.hpp>
#include <Network/NetworkManager.hpp>
#include <Physics/PhysicsManager.hpp>
#include <Graphics/TerrainManager.hpp>
#include <Logic/ScriptManager.hpp>

namespace dt {

QString Root::_VERSION = DUCTTAPE_VERSION;

// This list of new keywords exists to allow us fine-grained control over
// the creation and deletion of these managers.
Root::Root()
    : mCoreApplication(nullptr),
      mLogManager(new LogManager()),
      mResourceManager(new ResourceManager()),
      mInputManager(new InputManager()),
      mDisplayManager(new DisplayManager()),
      mStateManager(new StateManager()),
      mNavigationManager(new NavigationManager()),
      mNetworkManager(new NetworkManager()),
      mPhysicsManager(new PhysicsManager()),
      mTerrainManager(new TerrainManager()),
      mScriptManager(new ScriptManager()) {}

Root::~Root() {
    // Complementary to the constructor, we destroy the managers in reverse
    // order.
    delete mScriptManager;
    delete mTerrainManager;
    delete mPhysicsManager;
    delete mNetworkManager;
    delete mStateManager;
    delete mDisplayManager;
    delete mInputManager;
    delete mResourceManager;
    delete mLogManager;
}

Root& Root::GetInstance() {
    static Root instance;
    return instance;
}

void Root::Initialize(int argc, char** argv) {
    mCoreApplication = new QCoreApplication(argc, argv);
    Serializer::Initialize();

    mSfClock.Restart();

    mLogManager->Initialize();
    mResourceManager->Initialize();
    mDisplayManager->Initialize();
    // Do not initialize the InputManager.
    // The display manager does this when the window is created.
    mNavigationManager->Initialize();
    mNetworkManager->Initialize();
    mStateManager->Initialize();
    mPhysicsManager->Initialize();
    mTerrainManager->Initialize();
    mScriptManager->Initialize();
}

void Root::Deinitialize() {
    mScriptManager->Deinitialize();
    mTerrainManager->Deinitialize();
    mPhysicsManager->Deinitialize();
    mStateManager->Deinitialize();
    mNetworkManager->Deinitialize();
    // Do not deinitialize the InputManager (see above).
    mNavigationManager->Deinitialize();
    mDisplayManager->Deinitialize();
    mResourceManager->Deinitialize();
    mLogManager->Deinitialize();

    Serializer::Deinitialize();
    delete mCoreApplication;
}

double Root::GetTimeSinceInitialize() const {
    return mSfClock.GetElapsedTime().AsSeconds();
}

LogManager* Root::GetLogManager() {
    return mLogManager;
}

StateManager* Root::GetStateManager() {
    return mStateManager;
}

NavigationManager* Root::GetNavigationManager() {
    return mNavigationManager;
}

NetworkManager* Root::GetNetworkManager() {
    return mNetworkManager;
}

DisplayManager* Root::GetDisplayManager() {
    return mDisplayManager;
}

InputManager* Root::GetInputManager() {
    return mInputManager;
}

ResourceManager* Root::GetResourceManager() {
    return mResourceManager;
}

PhysicsManager* Root::GetPhysicsManager() {
    return mPhysicsManager;
}

ScriptManager* Root::GetScriptManager() {
    return mScriptManager;
}

TerrainManager* Root::GetTerrainManager() {
    return mTerrainManager;
}

}
