
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include <Scene/Component.hpp>

#include <Core/StringManager.hpp>
#include <Event/EventManager.hpp>
#include <Logic/ScriptManager.hpp>
#include <Scene/Node.hpp>
#include <Scene/Scene.hpp>
#include <Utils/Utils.hpp>

#include <OgreSceneManager.h>

namespace dt {

Component::Component(const QString& name, const QString& mesh_handle_d)
    : mName(name),
      mNode(nullptr),
      mMeshHandle(mesh_handle_d),
      mEntity(nullptr),
      mSceneNode(nullptr),
      mIsEnabled(false),
      mIsCreated(false) {
    // auto-generate the component name
    if(mName == "") {
        mName = "Component-" % Utils::ToString(StringManager::Get()->GetNextAutoId());
    }
}

Component::~Component() {}

const QString& Component::GetName() const {
    return mName;
}

void Component::HandleEvent(std::shared_ptr<Event> e) {}

void Component::OnCreate() {}

void Component::OnDestroy() {}

void Component::OnEnable() {}

void Component::OnDisable() {}

void Component::OnUpdate(double time_diff) {}

void Component::SetNode(Node* node) {
    mNode = node;
}

Node* Component::GetNode() {
    return mNode;
}

QScriptValue Component::GetScriptNode() {
    // Making QScriptValue from Node. Type conversion in C style only due to limitation of incomplete type.
    // return dt::ScriptManager::GetScriptEngine()->newQObject((QObject*)mNode);
    return dt::ScriptManager::Get()->GetScriptEngine()->newQObject(mNode);
}

void Component::Create() {
    if(!mIsCreated) {
        mIsCreated = true;
        _LoadDebugMesh();
        HideDebug();
        OnCreate();
        emit ComponentCreated();
        Enable();
    }
    EventManager::Get()->AddListener(this);
}

void Component::Destroy() {
    EventManager::Get()->RemoveListener(this);
    if(mIsCreated) {
        mIsCreated = false;
        Disable();
        emit ComponentDestroyed();
        OnDestroy();
        _DestroyDebugMesh();
    }
}

void Component::Enable() {
    if(!mIsEnabled) {
        mIsEnabled = true;
        emit ComponentEnabled();
        OnEnable();
    }
}

void Component::Disable() {
    if(mIsEnabled) {
        mIsEnabled = false;
        emit ComponentDisabled();
        OnDisable();
    }
}

bool Component::IsCreated() {
    return mIsCreated;
}

bool Component::IsEnabled() {
    return mIsEnabled;
}

void Component::HideDebug() {
    mEntity->setVisible(false);
}

void Component::ShowDebug() {
    mEntity->setVisible(true);
}

void Component::_LoadDebugMesh() {
    _DestroyDebugMesh();

    if(mMeshHandle.isEmpty()) {
        Logger::Get().Error("Component [" + mName + "]: Invalid handle for debug mesh.");
    }

    Ogre::SceneManager* scene_mgr = mNode->GetScene()->GetSceneManager();
    std::string node_name = Utils::ToStdString(GetNode()->GetName());
    mEntity = scene_mgr->createEntity(node_name + "-debug-mesh-entity-" + Utils::ToStdString(mName), Utils::ToStdString(mMeshHandle));
    mSceneNode = scene_mgr->getRootSceneNode()->createChildSceneNode(node_name + "-debug-mesh-scenenode-" + Utils::ToStdString(mName));
    mSceneNode->attachObject(mEntity);
    mEntity->setCastShadows(false);
}

void Component::_DestroyDebugMesh() {
    Ogre::SceneManager* scene_mgr = mNode->GetScene()->GetSceneManager();

    if(mEntity != nullptr)
        scene_mgr->destroyEntity(mEntity);

    if(mSceneNode != nullptr)
        scene_mgr->destroySceneNode(mSceneNode);
}

void Component::Update(double time_diff) {
    // set position of the debug mesh node.
    mSceneNode->setPosition(GetNode()->GetPosition(Node::SCENE));
    OnUpdate(time_diff);
}

} // namespace dt
