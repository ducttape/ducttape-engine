
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include <Scene/State.hpp>

namespace dt {

State::State() {}

void State::onDeinitialize() {}

void State::initialize() {
    Logger::get().info("Initializing state.");
    onInitialize();
}

void State::deinitialize() {
    // deinitialize all scenes
    while(mScenes.size() > 0) {
        deleteScene(mScenes.begin()->second->getName());
    }

    onDeinitialize();

    Logger::get().info("Deinitialized state.");
}

Scene::SceneSP State::addScene(Scene* scene) {
    QString key(scene->getName());
    Scene::SceneSP scene_sp(scene);
    mScenes.insert(std::make_pair(key, scene_sp));
    getScene(key)->initialize();
    //connect(this, SIGNAL(BeginFrame(double)), GetScene(key), SLOT(UpdateFrame(double)));
    return getScene(key);
}

Scene::SceneSP State::getScene(const QString name) {
    if(mScenes.find(name) != mScenes.end())
        return mScenes.find(name)->second;
    return Scene::SceneSP();
}

void State::deleteScene(const QString name) {
    if(getScene(name) == nullptr) {
        Logger::get().warning("Cannot delete scene " + name + ": There is no such scene.");
        return;
    }

    getScene(name)->deinitialize();
    mScenes.erase(mScenes.find(name));
}

void State::updateFrame(double simulation_frame_time) {
    updateSceneFrame(simulation_frame_time);
    updateStateFrame(simulation_frame_time);
}

void State::updateSceneFrame(double simulation_frame_time) {
    for(auto i = mScenes.begin();i != mScenes.end(); i++) {
        i->second->updateFrame(simulation_frame_time);
    }
}

} // namespace dt
