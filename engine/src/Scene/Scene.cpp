
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include <Scene/Scene.hpp>

#include <Event/BeginFrameEvent.hpp>
#include <Event/EventManager.hpp>
#include <Graphics/DisplayManager.hpp>
#include <Physics/PhysicsManager.hpp>

namespace dt {

Scene::Scene(const QString& name)
    : Node(name) {}

EventListener::Priority Scene::GetEventPriority() const {
    return EventListener::LOWEST;
}

void Scene::OnInitialize() {
    Logger::Get().Debug("Scene " + mName + " is being initialized.");
    EventManager::Get()->AddListener(this);
}

void Scene::OnDeinitialize() {
    Logger::Get().Debug("Scene " + mName + " is being deinitialized.");
    EventManager::Get()->RemoveListener(this);
}

Ogre::SceneManager* Scene::GetSceneManager() {
    return DisplayManager::Get()->GetSceneManager(mName);
}

bool Scene::_IsScene() {
    return true;
}

void Scene::HandleEvent(std::shared_ptr<Event> e) {
    if(e->GetType() == "DT_BEGINFRAMEEVENT") {
        OnUpdate((std::dynamic_pointer_cast<BeginFrameEvent>(e))->GetFrameTime());
    }
}

PhysicsWorld* Scene::GetPhysicsWorld() {
    PhysicsManager* mgr = PhysicsManager::Get();
    // create a world if none exists
    if(!mgr->HasWorld(mName)) {
        return mgr->AddWorld(new PhysicsWorld(mName, this));
    }
    return mgr->GetWorld(mName);
}

} // namespace dt
