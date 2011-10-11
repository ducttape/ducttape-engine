
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include <Scene/State.hpp>

namespace dt {

State::State() {}

void State::OnDeinitialize() {}

void State::Initialize() {
    Logger::Get().Info("Initializing state.");
    OnInitialize();
}

void State::Deinitialize() {
    // deinitialize all scenes
    while(mScenes.size() > 0) {
        DeleteScene(mScenes.begin()->second->GetName());
    }

    OnDeinitialize();

    Logger::Get().Info("Deinitialized state.");
}

Scene* State::AddScene(Scene* scene) {
    QString key(scene->GetName());
    mScenes.insert(key, scene);
    GetScene(key)->Initialize();
    //connect(this, SIGNAL(BeginFrame(double)), GetScene(key), SLOT(UpdateFrame(double)));
    return GetScene(key);
}

Scene* State::GetScene(const QString& name) {
    if(mScenes.find(name) != mScenes.end())
        return mScenes.find(name)->second;
    return nullptr;
}

void State::DeleteScene(const QString& name) {
    if(GetScene(name) == nullptr) {
        Logger::Get().Warning("Cannot delete scene " + name + ": There is no such scene.");
        return;
    }

    GetScene(name)->Deinitialize();
    mScenes.erase(mScenes.find(name));
}

void State::UpdateFrame(double simulation_frame_time) {
    UpdateSceneFrame(simulation_frame_time);
    UpdateStateFrame(simulation_frame_time);
}

void State::UpdateSceneFrame(double simulation_frame_time) {
    for(auto i = mScenes.begin();i != mScenes.end(); i++) {
        i->second->UpdateFrame(simulation_frame_time);
    }
}

} // namespace dt
