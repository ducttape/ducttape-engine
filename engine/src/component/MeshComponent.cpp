#include "MeshComponent.hpp"

#include "scene/Node.hpp"
#include "scene/Scene.hpp"
#include "utils/Logger.hpp"

namespace dt {

MeshComponent::MeshComponent(const std::string& name, const std::string& mesh_file)
    : Component(name) {
    mEntity = nullptr;
    mSceneNode = nullptr;
    mMeshFile = mesh_file;
    mAnimationState = nullptr;
}

void MeshComponent::HandleEvent(Event* e) {

}

void MeshComponent::OnCreate() {
    _LoadMesh();
}

void MeshComponent::OnDestroy() {
    _DestroyMesh();
}

void MeshComponent::OnUpdate(float time_diff) {
    // set position, rotation and scale of the node
    mSceneNode->setPosition(GetNode()->GetPosition(Node::SCENE));
    mSceneNode->setOrientation(GetNode()->GetRotation(Node::SCENE));
    mSceneNode->setScale(GetNode()->GetScale(Node::SCENE));


    if(mAnimationState != nullptr && mAnimationState->getEnabled()) {
        mAnimationState->addTime(time_diff);
    }
}

void MeshComponent::SetMeshFile(const std::string& mesh_file) {
    if(mesh_file != mMeshFile && IsCreated()) {
        // we got a new mesh; load it
        _LoadMesh();
    }
    mMeshFile = mesh_file;
}

const std::string& MeshComponent::GetMeshFile() const {
    return mMeshFile;
}

std::vector<std::string> MeshComponent::GetAvailableAnimations() {
    std::vector<std::string> result;

    if(mEntity == nullptr)
        return result;

    Ogre::AnimationStateIterator iter = mEntity->getAllAnimationStates()->getAnimationStateIterator();
    while(iter.hasMoreElements()) {
        result.push_back(iter.current()->second->getAnimationName());
        iter.moveNext();
    }
    return result;
}

void MeshComponent::SetAnimation(const std::string& animation_state) {
    if(mEntity != nullptr) {
        mAnimationState = mEntity->getAnimationState(animation_state);
        mAnimationState->setLoop(mLoopAnimation);
    } else {
        Logger::Get().Error("Cannot set animation of component " + GetName() + ": No entity loaded.");
    }
}

void MeshComponent::PlayAnimation() {
    if(mAnimationState != nullptr) {
        mAnimationState->setEnabled(true);
    } else {
        Logger::Get().Error("Cannot play animation of component " + GetName() + ": No animation set.");
    }
}

void MeshComponent::StopAnimation() {
    if(mAnimationState != nullptr) {
        mAnimationState->setEnabled(false);
        mAnimationState->setTimePosition(0);
    } else {
        Logger::Get().Error("Cannot stop animation of component " + GetName() + ": No animation set.");
    }
}

void MeshComponent::PauseAnimation() {
    if(mAnimationState != nullptr) {
        mAnimationState->setEnabled(false);
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


void MeshComponent::_LoadMesh() {
    // destroy existing mesh and scene node
    _DestroyMesh();

    if(mMeshFile == "") {
        Logger::Get().Error("MeshComponent ["+ mName + "]: Needs a mesh file.");
    }

    Ogre::SceneManager* scene_mgr = GetNode()->GetScene()->GetSceneManager();
    const std::string& nodename = GetNode()->GetName();
    mEntity = scene_mgr->createEntity(nodename + "-mesh-entity-" + mName, mMeshFile);
    mSceneNode = scene_mgr->getRootSceneNode()->createChildSceneNode(nodename + "-mesh-scenenode-" + mName);
    mSceneNode->attachObject(mEntity);
}

void MeshComponent::_DestroyMesh() {
    Ogre::SceneManager* scene_mgr = GetNode()->GetScene()->GetSceneManager();

    if(mEntity != nullptr)
        scene_mgr->destroyEntity(mEntity);

    if(mSceneNode != nullptr)
        scene_mgr->destroySceneNode(mSceneNode);
}

}
