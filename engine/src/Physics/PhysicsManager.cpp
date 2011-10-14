
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

void PhysicsManager::Initialize() {
}

void PhysicsManager::Deinitialize() {
    for(auto it = mWorlds.begin(); it != mWorlds.end(); ++it) {
        it->second->Deinitialize();
    }
    mWorlds.clear();
}

void PhysicsManager::UpdateFrame(double simulation_frame_time) {
    // step all worlds
    for(auto iter = mWorlds.begin(); iter != mWorlds.end(); ++iter) {
        iter->second->StepSimulation(simulation_frame_time);
    }
}

PhysicsManager* PhysicsManager::Get() {
    return Root::GetInstance().GetPhysicsManager();
}

bool PhysicsManager::HasWorld(const QString& name) {
    return mWorlds.count(name) > 0;
}

PhysicsWorld* PhysicsManager::AddWorld(PhysicsWorld* world) {
    QString name = world->GetName();
    mWorlds.insert(name, world);
    GetWorld(name)->Initialize();
    return GetWorld(name);
}

PhysicsWorld* PhysicsManager::GetWorld(const QString& name) {
    if(HasWorld(name))
        return mWorlds.find(name)->second;
    return nullptr;
}

}
