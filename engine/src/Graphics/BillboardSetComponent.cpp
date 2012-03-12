
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

BillboardSetComponent::BillboardSetComponent(const QString name, uint32_t pool_size,
                                             const QString file) :
    Component(name),
    mBillboardSet(nullptr),
    mPoolSize(pool_size),
    mImageFile(file),
    mSceneNode(nullptr),
    mTextureUnitState(nullptr) {}

void BillboardSetComponent::onInitialize() {
    mBillboardSet = getNode()->getScene()->getSceneManager()
                    ->createBillboardSet(Utils::toStdString(mName), mPoolSize);

    std::string material_name = Utils::toStdString(mName) + "_material";
    mMaterialPtr = Ogre::MaterialManager::getSingleton()
                                    .create(material_name, "General", true);
    mBillboardSet->setMaterial(mMaterialPtr);
    Ogre::Pass* pass = mMaterialPtr->getTechnique(0)->getPass(0);
    mTextureUnitState = pass->createTextureUnitState();

    // If a image file is given, create one billboard and use the image as texture.
    if(!mImageFile.isEmpty()) {
        mBillboardSet->createBillboard(0, 0, 0);
        setTextureFromFile(mImageFile);
    }

    // Commons default settings for a billboard.
    pass->setCullingMode(Ogre::CULL_NONE);   // No culling (triangles can be seen from both sides).
    pass->setSceneBlending(Ogre::SBT_TRANSPARENT_ALPHA); // Allow transparency from alpha channel.
    mMaterialPtr->setLightingEnabled(false);   // Disable lighting.

    mSceneNode = getNode()->getScene()->getSceneManager()->getRootSceneNode()
                 ->createChildSceneNode(Utils::toStdString(mName) + "_node");
    mSceneNode->attachObject(mBillboardSet);
}

void BillboardSetComponent::onDeinitialize() {
    Ogre::SceneManager* scene_mgr = getNode()->getScene()->getSceneManager();

    if(mBillboardSet != nullptr) {
        scene_mgr->destroyBillboardSet(mBillboardSet);
    }
    
    if(mSceneNode != nullptr) {
        scene_mgr->destroySceneNode(mSceneNode);
    }
}

void BillboardSetComponent::onEnable() {
    mBillboardSet->setVisible(true);
}

void BillboardSetComponent::onDisable() {
    mBillboardSet->setVisible(false);
}

void BillboardSetComponent::onUpdate(double time_diff) {
    mSceneNode->setPosition(getNode()->getPosition(Node::SCENE));
    mSceneNode->setOrientation(getNode()->getRotation(Node::SCENE));
    mSceneNode->setScale(getNode()->getScale(Node::SCENE));
}

Ogre::BillboardSet* BillboardSetComponent::getOgreBillboardSet() const {
    return mBillboardSet;
}

void BillboardSetComponent::setTextureFromFile(const QString file) {
    Ogre::TextureManager::getSingleton().load(Utils::toStdString(file), "General");
    mTextureUnitState->setTextureName(Utils::toStdString(file));
}

void BillboardSetComponent::setFaceCamera() {
    mBillboardSet->setBillboardType(Ogre::BBT_POINT);
}

void BillboardSetComponent::setOrientedCommon(const Ogre::Vector3& common_vector) {
    mBillboardSet->setBillboardType(Ogre::BBT_ORIENTED_COMMON);
    mBillboardSet->setCommonDirection(common_vector);
}

void BillboardSetComponent::setOrientedCommon(float x, float y, float z) {
    setOrientedCommon(Ogre::Vector3(x, y, z));
}

void BillboardSetComponent::setOrientedSelf() {
    mBillboardSet->setBillboardType(Ogre::BBT_ORIENTED_SELF);
}

void BillboardSetComponent::setPerpendicularCommon(const Ogre::Vector3& common_vector, const Ogre::Vector3& up_vector) {
    mBillboardSet->setBillboardType(Ogre::BBT_PERPENDICULAR_COMMON);
    mBillboardSet->setCommonDirection(common_vector);
    mBillboardSet->setCommonUpVector(up_vector);
}

void BillboardSetComponent::setPerpendicularSelf(const Ogre::Vector3& up_vector) {
    mBillboardSet->setBillboardType(Ogre::BBT_PERPENDICULAR_SELF);
    mBillboardSet->setCommonUpVector(up_vector);
}

void BillboardSetComponent::setPerpendicularSelf(float x, float y, float z) {
    setPerpendicularSelf(Ogre::Vector3(x, y, z));
}

void BillboardSetComponent::setDepthCheckEnabled(bool enabled) {
    mMaterialPtr->setDepthCheckEnabled(enabled);
}

}
