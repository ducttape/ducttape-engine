
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include <Scene/Scene.hpp>

#include <Graphics/DisplayManager.hpp>
#include <Physics/PhysicsManager.hpp>
#include <Gui/GuiManager.hpp>

namespace dt {

Scene::Scene(const QString name)
    : Node(name) {}

void Scene::onInitialize() {
    GuiManager::get()->setSceneManager(getSceneManager());

    Logger::get().debug("Scene " + mName + " is being initialized.");
}

void Scene::onDeinitialize() {
    GuiManager::get()->getRootWindow().removeAllChildren();
    GuiManager::get()->setSceneManager(nullptr);

    Logger::get().debug("Scene " + mName + " is being deinitialized.");
}

Ogre::SceneManager* Scene::getSceneManager() {
    return DisplayManager::get()->getSceneManager(mName);
}

bool Scene::_isScene() {
    return true;
}

void Scene::updateFrame(double simulation_frame_time) {
    onUpdate(simulation_frame_time);
}

PhysicsWorld::PhysicsWorldSP Scene::getPhysicsWorld() {
    PhysicsManager* mgr = PhysicsManager::get();
    // create a world if none exists
    if(!mgr->hasWorld(mName)) {
        return mgr->addWorld(new PhysicsWorld(mName, this));
    }
    return mgr->getWorld(mName);
}

} // namespace dt
