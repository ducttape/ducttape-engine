#include "Scene.hpp"

#include "Root.hpp"
#include "graphics/DisplayManager.hpp"

namespace dt {

Scene::Scene(const std::string& name) {
    mName = name;
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

}
