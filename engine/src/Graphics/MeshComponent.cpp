
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

MeshComponent::MeshComponent(const QString mesh_handle, const QString material_name, const QString name)
    : Component(name),
      mSceneNode(nullptr),
      mEntity(nullptr),
      mAnimationState(nullptr),
      mLoopAnimation(false),
      mCastShadows(true) {
    mMeshHandle = mesh_handle;
    mMaterialName = material_name;
}

void MeshComponent::onInitialize() {
    _loadMesh();
}

void MeshComponent::onDeinitialize() {
    _destroyMesh();
}

void MeshComponent::onEnable() {
    mEntity->setVisible(true);
}

void MeshComponent::onDisable() {
    mEntity->setVisible(false);
}

void MeshComponent::onUpdate(double time_diff) {
    // set position, rotation and scale of the node
    mSceneNode->setPosition(getNode()->getPosition(Node::SCENE));
    mSceneNode->setOrientation(getNode()->getRotation(Node::SCENE));
    mSceneNode->setScale(getNode()->getScale(Node::SCENE));

    if(mAnimationState != nullptr && mAnimationState->getEnabled()) {
        mAnimationState->addTime(time_diff);
    }
}

void MeshComponent::onSerialize(IOPacket& packet) {
    packet.stream(mMeshHandle, "mesh");
    packet.stream(mMaterialName, "material");
}

void MeshComponent::setMeshHandle(const QString mesh_handle) {
    if(mesh_handle != mMeshHandle && isInitialized()) {
        // we got a new mesh; load it
        _loadMesh();
    }
    mMeshHandle = mesh_handle;
}

const QString MeshComponent::getMeshHandle() const {
    return mMeshHandle;
}

std::vector<QString> MeshComponent::getAvailableAnimations() {
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

void MeshComponent::setAnimation(const QString animation_state) {
    if(mEntity != nullptr) {
        mAnimationState = mEntity->getAnimationState(Utils::toStdString(animation_state));
        mAnimationState->setLoop(mLoopAnimation);
    } else {
        Logger::get().error("Cannot set animation of component " + getName() + ": No entity loaded.");
    }
}

void MeshComponent::playAnimation() {
    if(mAnimationState != nullptr && mAnimationState->getEnabled() != true) {
        mAnimationState->setEnabled(true);
        emit animationPlayed();
    } else {
        Logger::get().error("Cannot play animation of component " + getName() + ": No animation set.");
    }
}

void MeshComponent::stopAnimation() {
    if(mAnimationState != nullptr && mAnimationState->getEnabled() != false) {
        mAnimationState->setEnabled(false);
        mAnimationState->setTimePosition(0);
        emit animationStopped();
    } else {
        Logger::get().error("Cannot stop animation of component " + getName() + ": No animation set.");
    }
}

void MeshComponent::pauseAnimation() {
    if(mAnimationState != nullptr && mAnimationState->getEnabled() != false) {
        mAnimationState->setEnabled(false);
        emit animationPaused();
    } else {
        Logger::get().error("Cannot pause animation of component " + getName() + ": No animation set.");
    }
}

void MeshComponent::setLoopAnimation(bool loop_animation) {
    mLoopAnimation = loop_animation;
    if(mAnimationState != nullptr) {
        mAnimationState->setLoop(mLoopAnimation);
    }
}

bool MeshComponent::getLoopAnimation() {
    return mLoopAnimation;
}

void MeshComponent::setMaterialName(const QString material_name) {
    mMaterialName = material_name;
    if(mEntity != nullptr && mMaterialName != "") {
        mEntity->setMaterialName(Utils::toStdString(material_name));
    }
}

Ogre::SceneNode* MeshComponent::getOgreSceneNode() const {
    return mSceneNode;
}

Ogre::Entity* MeshComponent::getOgreEntity() const {
    return mEntity;
}

void MeshComponent::setCastShadows(bool cast_shadows) {
    mCastShadows = cast_shadows;
    if(mEntity != nullptr) {
        mEntity->setCastShadows(mCastShadows);
    }
}

bool MeshComponent::getCastShadows() const {
    return mCastShadows;
}

void MeshComponent::_loadMesh() {
    // destroy existing mesh and scene node
    _destroyMesh();

    if(mMeshHandle == "") {
        Logger::get().error("MeshComponent ["+ mName + "]: Needs a mesh handle.");
    }

    Ogre::SceneManager* scene_mgr = getNode()->getScene()->getSceneManager();
    std::string nodename = Utils::toStdString(getNode()->getName());
    mEntity = scene_mgr->createEntity(nodename + "-mesh-entity-" + Utils::toStdString(mName),
                                                             Utils::toStdString(mMeshHandle));
    setMaterialName(mMaterialName);
    mSceneNode = scene_mgr->getRootSceneNode()->createChildSceneNode(nodename + "-mesh-scenenode-" + Utils::toStdString(mName));
    mSceneNode->attachObject(mEntity);
    setCastShadows(mCastShadows);
}

void MeshComponent::_destroyMesh() {
    Ogre::SceneManager* scene_mgr = getNode()->getScene()->getSceneManager();

    if(mEntity != nullptr)
        scene_mgr->destroyEntity(mEntity);

    if(mSceneNode != nullptr)
        scene_mgr->destroySceneNode(mSceneNode);
}

}
