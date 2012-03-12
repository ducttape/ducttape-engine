
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

Root& Root::getInstance() {
    static Root instance;
    return instance;
}

void Root::initialize(int argc, char** argv) {
    if(qApp){
        mCoreApplication = qApp;
    } else {
        mCoreApplication = new QCoreApplication(argc, argv);
    }

    Serializer::initialize();

    mSfClock.restart();

    mLogManager->initialize();
    mResourceManager->initialize();
    mDisplayManager->initialize();
    // Do not initialize the InputManager.
    // The display manager does this when the window is created.
    mNetworkManager->initialize();
    mStateManager->initialize();
    mPhysicsManager->initialize();
    mTerrainManager->initialize();
    mScriptManager->initialize();
}

void Root::deinitialize() {
    mScriptManager->deinitialize();
    mTerrainManager->deinitialize();
    mPhysicsManager->deinitialize();
    mStateManager->deinitialize();
    mNetworkManager->deinitialize();
    // Do not deinitialize the InputManager (see above).
    mDisplayManager->deinitialize();
    mResourceManager->deinitialize();
    mLogManager->deinitialize();

    Serializer::deinitialize();
    delete mCoreApplication;
}

double Root::getTimeSinceInitialize() const {
    return mSfClock.getElapsedTime().asSeconds();
}

LogManager* Root::getLogManager() {
    return mLogManager;
}

StateManager* Root::getStateManager() {
    return mStateManager;
}

NetworkManager* Root::getNetworkManager() {
    return mNetworkManager;
}

DisplayManager* Root::getDisplayManager() {
    return mDisplayManager;
}

InputManager* Root::getInputManager() {
    return mInputManager;
}

ResourceManager* Root::getResourceManager() {
    return mResourceManager;
}

PhysicsManager* Root::getPhysicsManager() {
    return mPhysicsManager;
}

ScriptManager* Root::getScriptManager() {
    return mScriptManager;
}

TerrainManager* Root::getTerrainManager() {
    return mTerrainManager;
}

}
