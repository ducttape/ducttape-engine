
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include <Graphics/ParticleSystemComponent.hpp>

#include <Scene/Node.hpp>
#include <Scene/Scene.hpp>
#include <Utils/Utils.hpp>

#include <OgreParticleAffector.h>
#include <OgreSceneManager.h>

namespace dt {

ParticleSystemComponent::ParticleSystemComponent(const QString name)
    : Component(name),
      mSceneNode(nullptr),
      mParticleSystem(nullptr),
      mParticleCountLimit(1000) {}

void ParticleSystemComponent::setParticleCountLimit(uint32_t limit) {
    mParticleCountLimit = limit;
    if(isInitialized()) {
        mParticleSystem->setParticleQuota(mParticleCountLimit);
    }
}

uint32_t ParticleSystemComponent::getParticleCountLimit() const {
    return mParticleCountLimit;
}

void ParticleSystemComponent::setMaterialName(const QString material_name) {
    mMaterialName = material_name;
    if(isInitialized() && mParticleSystem != nullptr) {
        mParticleSystem->setMaterialName(Utils::toStdString(mMaterialName));
    }
}

const QString ParticleSystemComponent::getMaterialName() const {
    return mMaterialName;
}

Ogre::ParticleEmitter* ParticleSystemComponent::addEmitter(const QString name, const QString type) {
    Ogre::ParticleEmitter* e = mParticleSystem->addEmitter(Utils::toStdString(type));
    mParticleEmitters[name] = e;
    return e;
}

Ogre::ParticleEmitter* ParticleSystemComponent::getEmitter(const QString name) {
    if(mParticleEmitters.count(name) == 0)
        return nullptr;
    return mParticleEmitters[name];
}

Ogre::ParticleAffector* ParticleSystemComponent::addAffector(const QString name, const QString type) {
    Ogre::ParticleAffector* a = mParticleSystem->addAffector(Utils::toStdString(type));
    mParticleAffectors[name] = a;
    return a;
}

Ogre::ParticleAffector* ParticleSystemComponent::getAffector(const QString name) {
    if(mParticleAffectors.count(name) == 0)
        return nullptr;
    return mParticleAffectors[name];
}

Ogre::ParticleAffector* ParticleSystemComponent::addScalerAffector(const QString name, float rate) {
    Ogre::ParticleAffector* a = addAffector(name, "Scaler");
    a->setParameter("rate", Utils::toStdString(Utils::toString(rate)));
    return a;
}

Ogre::ParticleAffector* ParticleSystemComponent::addLinearForceAffector(const QString name, Ogre::Vector3 force) {
    Ogre::ParticleAffector* a = addAffector(name, "LinearForce");
    a->setParameter("force_vector", Utils::toStdString(Utils::toString(force.x)) + " " + \
                    Utils::toStdString(Utils::toString(force.y)) + " " + \
                    Utils::toStdString(Utils::toString(force.z)));
    return a;
}

Ogre::ParticleSystem* ParticleSystemComponent::getOgreParticleSystem() {
    if(!isInitialized())
        return nullptr;
    return mParticleSystem;
}


void ParticleSystemComponent::onInitialize() {
    if(mNode != nullptr) {
        Ogre::SceneManager* scene_mgr = mNode->getScene()->getSceneManager();
        mSceneNode = scene_mgr->getRootSceneNode()->createChildSceneNode(Utils::toStdString(mName) + "-node");
        mParticleSystem = scene_mgr->createParticleSystem(Utils::toStdString(mName) + "-system", mParticleCountLimit);
        if(mMaterialName != "")
            mParticleSystem->setMaterialName(Utils::toStdString(mMaterialName));
        mSceneNode->attachObject(mParticleSystem);
        mParticleSystem->setDefaultDimensions(1, 1);
    } else {
        Logger::get().warning("Cannot create ParticleSystemComponent " + mName + ": not attached to a Node.");
    }
}

void ParticleSystemComponent::onDeinitialize() {
    if(mNode != nullptr && mParticleSystem != nullptr) {
        mNode->getScene()->getSceneManager()->destroyParticleSystem(mParticleSystem);
        mParticleSystem = nullptr;
    }
    if(mNode != nullptr && mSceneNode != nullptr) {
        mNode->getScene()->getSceneManager()->destroySceneNode(mSceneNode);
        mSceneNode = nullptr;
    }
}

void ParticleSystemComponent::onEnable() {
    mParticleSystem->setEmitting(true);
}

void ParticleSystemComponent::onDisable() {
    mParticleSystem->setEmitting(false);
}

void ParticleSystemComponent::onUpdate(double time_diff) {
    mSceneNode->setPosition(mNode->getPosition(Node::SCENE));
    mSceneNode->setOrientation(mNode->getRotation(Node::SCENE));
    mSceneNode->setScale(mNode->getScale(Node::SCENE));
}

}
