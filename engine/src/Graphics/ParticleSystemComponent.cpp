
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

ParticleSystemComponent::ParticleSystemComponent(const QString& name)
    : Component(name),
      mSceneNode(nullptr),
      mParticleSystem(nullptr),
      mParticleCountLimit(1000) {}

void ParticleSystemComponent::SetParticleCountLimit(uint32_t limit) {
    mParticleCountLimit = limit;
    if(IsCreated()) {
        mParticleSystem->setParticleQuota(mParticleCountLimit);
    }
}

uint32_t ParticleSystemComponent::GetParticleCountLimit() const {
    return mParticleCountLimit;
}

void ParticleSystemComponent::SetMaterialName(const QString& material_name) {
    mMaterialName = material_name;
    if(IsCreated() && mParticleSystem != nullptr) {
        mParticleSystem->setMaterialName(Utils::ToStdString(mMaterialName));
    }
}

const QString& ParticleSystemComponent::GetMaterialName() const {
    return mMaterialName;
}

Ogre::ParticleEmitter* ParticleSystemComponent::AddEmitter(const QString& name, const QString& type) {
    Ogre::ParticleEmitter* e = mParticleSystem->addEmitter(Utils::ToStdString(type));
    mParticleEmitters[name] = e;
    return e;
}

Ogre::ParticleEmitter* ParticleSystemComponent::GetEmitter(const QString& name) {
    if(mParticleEmitters.count(name) == 0)
        return nullptr;
    return mParticleEmitters[name];
}

Ogre::ParticleAffector* ParticleSystemComponent::AddAffector(const QString& name, const QString& type) {
    Ogre::ParticleAffector* a = mParticleSystem->addAffector(Utils::ToStdString(type));
    mParticleAffectors[name] = a;
    return a;
}

Ogre::ParticleAffector* ParticleSystemComponent::GetAffector(const QString& name) {
    if(mParticleAffectors.count(name) == 0)
        return nullptr;
    return mParticleAffectors[name];
}

Ogre::ParticleAffector* ParticleSystemComponent::AddScalerAffector(const QString& name, float rate) {
    Ogre::ParticleAffector* a = AddAffector(name, "Scaler");
    a->setParameter("rate", Utils::ToStdString(Utils::ToString(rate)));
    return a;
}

Ogre::ParticleAffector* ParticleSystemComponent::AddLinearForceAffector(const QString& name, Ogre::Vector3 force) {
    Ogre::ParticleAffector* a = AddAffector(name, "LinearForce");
    a->setParameter("force_vector", Utils::ToStdString(Utils::ToString(force.x)) + " " + Utils::ToStdString(Utils::ToString(force.y)) + " " + Utils::ToStdString(Utils::ToString(force.z)));
    return a;
}

Ogre::ParticleSystem* ParticleSystemComponent::GetOgreParticleSystem() {
    if(!IsCreated())
        return nullptr;
    return mParticleSystem;
}


void ParticleSystemComponent::OnCreate() {
    if(mNode != nullptr) {
        Ogre::SceneManager* scene_mgr = mNode->GetScene()->GetSceneManager();
        mSceneNode = scene_mgr->getRootSceneNode()->createChildSceneNode(Utils::ToStdString(mName) + "-node");
        mParticleSystem = scene_mgr->createParticleSystem(Utils::ToStdString(mName) + "-system", mParticleCountLimit);
        if(mMaterialName != "")
            mParticleSystem->setMaterialName(Utils::ToStdString(mMaterialName));
        mSceneNode->attachObject(mParticleSystem);
        mParticleSystem->setDefaultDimensions(1, 1);
    } else {
        Logger::Get().Warning("Cannot create ParticleSystemComponent " + mName + ": not attached to a Node.");
    }
}

void ParticleSystemComponent::OnDestroy() {
    if(mNode != nullptr && mParticleSystem != nullptr) {
        mNode->GetScene()->GetSceneManager()->destroyParticleSystem(mParticleSystem);
        mParticleSystem = nullptr;
    }
    if(mNode != nullptr && mSceneNode != nullptr) {
        mNode->GetScene()->GetSceneManager()->destroySceneNode(mSceneNode);
        mSceneNode = nullptr;
    }
}

void ParticleSystemComponent::OnEnable() {
    mParticleSystem->setEmitting(true);
}

void ParticleSystemComponent::OnDisable() {
    mParticleSystem->setEmitting(false);
}

void ParticleSystemComponent::OnUpdate(double time_diff) {
    mSceneNode->setPosition(mNode->GetPosition(Node::SCENE));
    mSceneNode->setOrientation(mNode->GetRotation(Node::SCENE));
    mSceneNode->setScale(mNode->GetScale(Node::SCENE));
}

}
