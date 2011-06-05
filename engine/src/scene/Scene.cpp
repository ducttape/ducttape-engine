#include "Scene.hpp"

namespace dt {

Scene::Scene() {}

Ogre::SceneManager* Scene::GetSceneManager() {
    return mSceneManager;
}

bool Scene::_IsScene() {
    return true;
}

}
