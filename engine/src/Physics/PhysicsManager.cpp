
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include <Physics/PhysicsManager.hpp>

namespace dt {

PhysicsManager::PhysicsManager() {}

void PhysicsManager::Initialize() {
    EventManager::Get()->AddListener(this);
}

void PhysicsManager::Deinitialize() {
    for(auto it = mWorlds.begin(); it != mWorlds.end(); ++it) {
        it->second->Deinitialize();
    }
    mWorlds.clear();
    EventManager::Get()->RemoveListener(this);
}

void PhysicsManager::HandleEvent(std::shared_ptr<Event> e) {
   if(e->GetType() == "DT_BEGINFRAMEEVENT") {
       double time_diff = std::dynamic_pointer_cast<dt::BeginFrameEvent>(e)->GetFrameTime();

       // step all worlds
       for(auto iter = mWorlds.begin(); iter != mWorlds.end(); ++iter) {
           iter->second->StepSimulation(time_diff);
       }
   }
}

EventListener::Priority PhysicsManager::GetEventPriority() const {
    // Set a high priority to update the physics world before
    // all the PhysicsBodyComponents are updated, so they do
    // not lag behind.
    return EventListener::HIGHEST;
}


PhysicsManager* PhysicsManager::Get() {
    return Root::GetInstance().GetPhysicsManager();
}

bool PhysicsManager::HasWorld(const std::string& name) {
    return mWorlds.count(name) > 0;
}

PhysicsWorld* PhysicsManager::AddWorld(PhysicsWorld* world) {
    std::string name = world->GetName();
    mWorlds.insert(name, world);
    GetWorld(name)->Initialize();
    return GetWorld(name);
}

PhysicsWorld* PhysicsManager::GetWorld(const std::string& name) {
    if(HasWorld(name))
        return mWorlds.find(name)->second;
    return nullptr;
}

}
