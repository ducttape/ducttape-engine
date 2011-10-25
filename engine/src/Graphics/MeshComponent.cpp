
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include <Graphics/MeshComponent.hpp>

#include <Scene/Node.hpp>
#include <Scene/Scene.hpp>
#include <Utils/Utils.hpp>

#include <OgreSceneManager.h>

namespace dt {

MeshComponent::MeshComponent(const QString& mesh_handle, const QString& material_name, const QString& name)
    : Component(name),
      mSceneNode(nullptr),
      mEntity(nullptr),
      mAnimationState(nullptr),
      mLoopAnimation(false),
      mCastShadows(true) {
    mMeshHandle = mesh_handle;
    mMaterialName = material_name;
}

void MeshComponent::OnCreate() {
    _LoadMesh();
}

void MeshComponent::OnDestroy() {
    _DestroyMesh();
}

void MeshComponent::OnEnable() {
    mEntity->setVisible(true);
}

void MeshComponent::OnDisable() {
    mEntity->setVisible(false);
}

void MeshComponent::OnUpdate(double time_diff) {
    // set position, rotation and scale of the node
    mSceneNode->setPosition(GetNode()->GetPosition(Node::SCENE));
    mSceneNode->setOrientation(GetNode()->GetRotation(Node::SCENE));
    mSceneNode->setScale(GetNode()->GetScale(Node::SCENE));

    if(mAnimationState != nullptr && mAnimationState->getEnabled()) {
        mAnimationState->addTime(time_diff);
    }
}

void MeshComponent::SetMeshHandle(const QString& mesh_handle) {
    if(mesh_handle != mMeshHandle && IsCreated()) {
        // we got a new mesh; load it
        _LoadMesh();
    }
    mMeshHandle = mesh_handle;
}

const QString& MeshComponent::GetMeshHandle() const {
    return mMeshHandle;
}

std::vector<QString> MeshComponent::GetAvailableAnimations() {
    std::vector<QString> result;

    if(mEntity == nullptr)
        return result;

    Ogre::AnimationStateIterator iter(mEntity->getAllAnimationStates()->getAnimationStateIterator());
    while(iter.hasMoreElements()) {
        result.push_back(QString(iter.current()->second->getAnimationName().c_str()));
        iter.moveNext();
    }
    return result;
}

void MeshComponent::SetAnimation(const QString& animation_state) {
    if(mEntity != nullptr) {
        mAnimationState = mEntity->getAnimationState(Utils::ToStdString(animation_state));
        mAnimationState->setLoop(mLoopAnimation);
    } else {
        Logger::Get().Error("Cannot set animation of component " + GetName() + ": No entity loaded.");
    }
}

void MeshComponent::PlayAnimation() {
    if(mAnimationState != nullptr && mAnimationState->getEnabled() != true) {
        mAnimationState->setEnabled(true);
        emit AnimationPlayed();
    } else {
        Logger::Get().Error("Cannot play animation of component " + GetName() + ": No animation set.");
    }
}

void MeshComponent::StopAnimation() {
    if(mAnimationState != nullptr && mAnimationState->getEnabled() != false) {
        mAnimationState->setEnabled(false);
        mAnimationState->setTimePosition(0);
        emit AnimationStopped();
    } else {
        Logger::Get().Error("Cannot stop animation of component " + GetName() + ": No animation set.");
    }
}

void MeshComponent::PauseAnimation() {
    if(mAnimationState != nullptr && mAnimationState->getEnabled() != false) {
        mAnimationState->setEnabled(false);
        emit AnimationPaused();
    } else {
        Logger::Get().Error("Cannot pause animation of component " + GetName() + ": No animation set.");
    }
}

void MeshComponent::SetLoopAnimation(bool loop_animation) {
    mLoopAnimation = loop_animation;
    if(mAnimationState != nullptr) {
        mAnimationState->setLoop(mLoopAnimation);
    }
}

bool MeshComponent::GetLoopAnimation() {
    return mLoopAnimation;
}

void MeshComponent::SetMaterialName(const QString& material_name) {
    mMaterialName = material_name;
    if(mEntity != nullptr && mMaterialName != "") {
        mEntity->setMaterialName(Utils::ToStdString(material_name));
    }
}

Ogre::SceneNode* MeshComponent::GetOgreSceneNode() const {
    return mSceneNode;
}

Ogre::Entity* MeshComponent::GetOgreEntity() const {
    return mEntity;
}

void MeshComponent::SetCastShadows(bool cast_shadows) {
    mCastShadows = cast_shadows;
    if(mEntity != nullptr) {
        mEntity->setCastShadows(mCastShadows);
    }
}

bool MeshComponent::GetCastShadows() const {
    return mCastShadows;
}

void MeshComponent::_LoadMesh() {
    // destroy existing mesh and scene node
    _DestroyMesh();

    if(mMeshHandle == "") {
        Logger::Get().Error("MeshComponent ["+ mName + "]: Needs a mesh handle.");
    }

    Ogre::SceneManager* scene_mgr = GetNode()->GetScene()->GetSceneManager();
    std::string nodename = Utils::ToStdString(GetNode()->GetName());
    mEntity = scene_mgr->createEntity(nodename + "-mesh-entity-" + Utils::ToStdString(mName), Utils::ToStdString(mMeshHandle));
    SetMaterialName(mMaterialName);
    mSceneNode = scene_mgr->getRootSceneNode()->createChildSceneNode(nodename + "-mesh-scenenode-" + Utils::ToStdString(mName));
    mSceneNode->attachObject(mEntity);
    SetCastShadows(mCastShadows);
}

void MeshComponent::_DestroyMesh() {
    Ogre::SceneManager* scene_mgr = GetNode()->GetScene()->GetSceneManager();

    if(mEntity != nullptr)
        scene_mgr->destroyEntity(mEntity);

    if(mSceneNode != nullptr)
        scene_mgr->destroySceneNode(mSceneNode);
}

}
