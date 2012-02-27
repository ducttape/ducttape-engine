
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include <Graphics/BillboardSetComponent.hpp>

#include <Scene/Node.hpp>
#include <Scene/Scene.hpp>
#include <Utils/Utils.hpp>

#include <OgreSceneManager.h>
#include <OgreBillboardSet.h>
#include <OgreBillboard.h>
#include <OgreRectangle.h>
#include <OgrePass.h>

#include <cstdint>

namespace dt {

BillboardSetComponent::BillboardSetComponent(const QString& name, uint32_t pool_size,
                                             const QString& file) : 
    Component(name),
    mBillboardSet(nullptr),
    mPoolSize(pool_size),
    mImageFile(file),
    mSceneNode(nullptr),
    mTextureUnitState(nullptr) {}

void BillboardSetComponent::OnInitialize() {
    mBillboardSet = GetNode()->GetScene()->GetSceneManager()
                    ->createBillboardSet(Utils::ToStdString(mName), mPoolSize);

    std::string material_name = Utils::ToStdString(mName) + "_material"; 
    mMaterialPtr = Ogre::MaterialManager::getSingleton()
                                    .create(material_name, "General", true);
    mBillboardSet->setMaterial(mMaterialPtr);
    Ogre::Pass* pass = mMaterialPtr->getTechnique(0)->getPass(0);
    mTextureUnitState = pass->createTextureUnitState();

    // If a image file is given, create one billboard and use the image as texture.
    if(!mImageFile.isEmpty()) {
        mBillboardSet->createBillboard(0, 0, 0);
        SetTextureFromFile(mImageFile);
    }

    // Commons default settings for a billboard.
    pass->setCullingMode(Ogre::CULL_NONE);   // No culling (triangles can be seen from both sides).
    pass->setSceneBlending(Ogre::SBT_TRANSPARENT_ALPHA); // Allow transparency from alpha channel.
    mMaterialPtr->setLightingEnabled(false);   // Disable lighting.

    mSceneNode = GetNode()->GetScene()->GetSceneManager()->getRootSceneNode()
                 ->createChildSceneNode(Utils::ToStdString(mName) + "_node");
    mSceneNode->attachObject(mBillboardSet);
}

void BillboardSetComponent::OnDeinitialize() {
    Ogre::SceneManager* scene_mgr = GetNode()->GetScene()->GetSceneManager();

    if(mBillboardSet != nullptr) {
        scene_mgr->destroyBillboardSet(mBillboardSet);
    }
    
    if(mSceneNode != nullptr) {
        scene_mgr->destroySceneNode(mSceneNode);
    }
}

void BillboardSetComponent::OnEnable() {
    mBillboardSet->setVisible(true);
}

void BillboardSetComponent::OnDisable() {
    mBillboardSet->setVisible(false);
}

void BillboardSetComponent::OnUpdate(double time_diff) {
    mSceneNode->setPosition(GetNode()->GetPosition(Node::SCENE));
    mSceneNode->setOrientation(GetNode()->GetRotation(Node::SCENE));
    mSceneNode->setScale(GetNode()->GetScale(Node::SCENE));
}

Ogre::BillboardSet* BillboardSetComponent::GetOgreBillboardSet() const {
    return mBillboardSet;
}

void BillboardSetComponent::SetTextureFromFile(const QString& file) {
    Ogre::TextureManager::getSingleton().load(Utils::ToStdString(file), "General");
    mTextureUnitState->setTextureName(Utils::ToStdString(file));
}

void BillboardSetComponent::SetFaceCamera() {
    mBillboardSet->setBillboardType(Ogre::BBT_POINT);
}

void BillboardSetComponent::SetOrientedCommon(const Ogre::Vector3& common_vector) {
    mBillboardSet->setBillboardType(Ogre::BBT_ORIENTED_COMMON);
    mBillboardSet->setCommonDirection(common_vector);
}

void BillboardSetComponent::SetOrientedCommon(float x, float y, float z) {
    SetOrientedCommon(Ogre::Vector3(x, y, z));
}

void BillboardSetComponent::SetOrientedSelf() {
    mBillboardSet->setBillboardType(Ogre::BBT_ORIENTED_SELF);
}

void BillboardSetComponent::SetPerpendicularCommon(const Ogre::Vector3& common_vector, const Ogre::Vector3& up_vector) {
    mBillboardSet->setBillboardType(Ogre::BBT_PERPENDICULAR_COMMON);
    mBillboardSet->setCommonDirection(common_vector);
    mBillboardSet->setCommonUpVector(up_vector);
}

void BillboardSetComponent::SetPerpendicularSelf(const Ogre::Vector3& up_vector) {
    mBillboardSet->setBillboardType(Ogre::BBT_PERPENDICULAR_SELF);
    mBillboardSet->setCommonUpVector(up_vector);
}

void BillboardSetComponent::SetPerpendicularSelf(float x, float y, float z) {
    SetPerpendicularSelf(Ogre::Vector3(x, y, z));
}

void BillboardSetComponent::setDepthCheckEnabled(bool enabled) {
    mMaterialPtr->setDepthCheckEnabled(enabled);
}

}
