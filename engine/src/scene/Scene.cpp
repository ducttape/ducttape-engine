
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include "Scene.hpp"

#include "Root.hpp"
#include "event/BeginFrameEvent.hpp"
#include "graphics/DisplayManager.hpp"

namespace dt {

Scene::Scene(const std::string& name) {
    mName = name;
    // EventManager::Get()->AddListener(this);
}

Scene::~Scene() {
    // EventManager::Get()->RemoveListener(this);
}

Ogre::SceneManager* Scene::GetSceneManager() {
    return DisplayManager::Get()->GetSceneManager(mName);
}

bool Scene::_IsScene() {
    return true;
}

const std::string& Scene::GetName() {
    return mName;
}

void Scene::HandleEvent(std::shared_ptr<Event> e) {
    if(e->GetType() == "DT_BEGINFRAMEEVENT") {
        OnUpdate((std::dynamic_pointer_cast<BeginFrameEvent>(e))->GetFrameTime());
    }
}

}
