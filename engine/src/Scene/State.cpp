
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include <Scene/State.hpp>

#include <Event/EventManager.hpp>

namespace dt {

State::State() {}

void State::HandleEvent(std::shared_ptr<Event> e) {}

void State::OnDeinitialize() {}

void State::Initialize() {
    EventManager::Get()->AddListener(this);
    OnInitialize();
}

void State::Deinitialize() {
    // deinitialize all scenes
    while(mScenes.size() > 0) {
        DeleteScene(mScenes.begin()->second->GetName());
    }

    OnDeinitialize();
    EventManager::Get()->RemoveListener(this);
}

Scene* State::AddScene(Scene* scene) {
    QString key(scene->GetName());
    mScenes.insert(key, scene);
    GetScene(key)->Initialize();
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

} // namespace dt
