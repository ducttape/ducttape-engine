#include "Scene.hpp"

#include "Root.hpp"
#include "event/BeginFrameEvent.hpp"
#include "graphics/DisplayManager.hpp"

namespace dt {

Scene::Scene(const std::string& name) {
    mName = name;
    // Root::get_mutable_instance().GetEventManager()->AddListener(this);
}

Scene::~Scene() {
    // Root::get_mutable_instance().GetEventManager()->RemoveListener(this);
}

Ogre::SceneManager* Scene::GetSceneManager() {
    return Root::get_mutable_instance().GetDisplayManager()->GetSceneManager(mName);
}

bool Scene::_IsScene() {
    return true;
}

const std::string& Scene::GetName() {
    return mName;
}

void Scene::HandleEvent(Event* e) {
    if(e->GetType() == "DT_BEGINFRAMEEVENT") {
        OnUpdate(((BeginFrameEvent*)e)->GetFrameTime() / 1000.f);
    }
}

}
