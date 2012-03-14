
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include <Physics/PhysicsManager.hpp>

#include <Core/Root.hpp>

namespace dt {

PhysicsManager::PhysicsManager() {}

void PhysicsManager::initialize() {
}

void PhysicsManager::deinitialize() {
    for(auto it = mWorlds.begin(); it != mWorlds.end(); ++it) {
        it->second->deinitialize();
    }
    mWorlds.clear();
}

void PhysicsManager::updateFrame(double simulation_frame_time) {
    // step all worlds
    for(auto iter = mWorlds.begin(); iter != mWorlds.end(); ++iter) {
        iter->second->stepSimulation(simulation_frame_time);
    }
}

PhysicsManager* PhysicsManager::get() {
    return Root::getInstance().getPhysicsManager();
}

bool PhysicsManager::hasWorld(const QString name) {
    return mWorlds.count(name) > 0;
}

PhysicsWorld* PhysicsManager::addWorld(PhysicsWorld* world) {
    QString name = world->getName();
    PhysicsWorld::PhysicsWorldSP world_sp(world);
    mWorlds.insert(std::make_pair(name, world_sp));
    getWorld(name)->initialize();
    return getWorld(name);
}

PhysicsWorld* PhysicsManager::getWorld(const QString name) {
    if(hasWorld(name))
        return mWorlds.find(name)->second.get();
    return nullptr;
}

}
