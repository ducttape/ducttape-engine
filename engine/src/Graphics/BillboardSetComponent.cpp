// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include "Graphics/BillboardSetComponent.hpp"

#include <Scene/Node.hpp>
#include <Scene/Scene.hpp>

#include <OgreSceneManager.h>
#include <OgreBillboardSet.h>
#include <OgreRectangle.h>

#include <cstdint>
#include <OgreBillboard.h>

namespace dt {

BillboardSetComponent::BillboardSetComponent (const QString& name, uint32_t poolSize,
        const QString& file)
        : Component (name),
        mBillboardSet (nullptr),
        mPoolSize (poolSize),
        mImageFile(file),
        mSceneNode (nullptr),
        mTextureUnitState (nullptr) {}

void BillboardSetComponent::OnCreate() {
    mBillboardSet = GetNode()->GetScene()->GetSceneManager()
                    ->createBillboardSet(mName.toStdString(),mPoolSize);

    std::string materialName = mName.toStdString() + "-material";
    Ogre::MaterialPtr materialPtr = Ogre::MaterialManager::getSingleton()
                                    .create(materialName, "General", true);
    mBillboardSet->setMaterial(materialPtr);
    Ogre::Pass* mPass = materialPtr->getTechnique(0)->getPass(0);
    mTextureUnitState = mPass->createTextureUnitState();

    //if a image file is given, create one billboard and use the image as texture
    if (!mImageFile.isEmpty()) {
        mBillboardSet->createBillboard(GetNode()->GetPosition());
        SetTextureFromFile(mImageFile);
    }

    // commons default settings for a billboard
    mPass->setCullingMode(Ogre::CULL_NONE);   // No culling (triangles can be seen from both sides)
    mPass->setSceneBlending(Ogre::SBT_TRANSPARENT_ALPHA); // Allow transparency from alpha channel
    materialPtr->setLightingEnabled(false);   // Disable lighting
    materialPtr->setDepthCheckEnabled(false); // No depth check

    mSceneNode = GetNode()->GetScene()->GetSceneManager()->getRootSceneNode()
                 ->createChildSceneNode(mName.toStdString() + "-node");
    mSceneNode->attachObject(mBillboardSet);
}

void BillboardSetComponent::OnDestroy() {
    Ogre::SceneManager* scene_mgr = GetNode()->GetScene()->GetSceneManager();

    if (mBillboardSet != nullptr)
        scene_mgr->destroyBillboardSet(mBillboardSet);

    if (mSceneNode != nullptr)
        scene_mgr->destroySceneNode(mSceneNode);
}

void BillboardSetComponent::OnEnable() {
    mBillboardSet->setVisible(true);
}

void BillboardSetComponent::OnDisable() {
    mBillboardSet->setVisible(false);
}

void BillboardSetComponent::OnUpdate (double time_diff) {
    mSceneNode->setPosition(GetNode()->GetPosition (Node::SCENE));
    mSceneNode->setOrientation(GetNode()->GetRotation(Node::SCENE));
    mSceneNode->setScale(GetNode()->GetScale (Node::SCENE));
}

Ogre::BillboardSet* BillboardSetComponent::GetOgreBillboardSet() const {
    return mBillboardSet;
}

void BillboardSetComponent::SetTextureFromFile(const QString& file)
{
    //open and read the file
    std::ifstream ifs(file.toStdString(), std::ios::binary|std::ios::in);
    if (ifs.is_open())
    {
        Ogre::String textureName = mName.toStdString() + "-texture";
        //erase old texture if exist
        Ogre::TextureManager::getSingleton().remove(textureName);
        QString extension;
        uint extensionIndex = file.lastIndexOf(".");
        extension = file.mid(extensionIndex+1);
        Ogre::FileStreamDataStream* fileStream = new Ogre::FileStreamDataStream(file.toStdString(),
                                                                                &ifs, false);
        Ogre::DataStreamPtr data_stream(fileStream);
        Ogre::Image img;
        img.load(data_stream, extension.toStdString());
        Ogre::TextureManager::getSingleton().loadImage(textureName,
                Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, img,
                Ogre::TEX_TYPE_2D, 0, 1.0f);
        mTextureUnitState->setTextureName(textureName);

        //clean
        data_stream->close();
        ifs.close();
    }
    else
        dt::Logger::Get().Warning("Couldn't open file: " + file);
}

void BillboardSetComponent::SetFaceCamera()
{
    mBillboardSet->setBillboardType(Ogre::BBT_POINT);
}

void BillboardSetComponent::SetOrientedCommon(const Ogre::Vector3& commonVector)
{
    mBillboardSet->setBillboardType(Ogre::BBT_ORIENTED_COMMON);
    mBillboardSet->setCommonDirection(commonVector);
}

void BillboardSetComponent::SetOrientedSelf()
{
    mBillboardSet->setBillboardType(Ogre::BBT_ORIENTED_SELF);
}

void BillboardSetComponent::SetPerpendicularCommon(const Ogre::Vector3& commonVector, const Ogre::Vector3& upVector)
{
    mBillboardSet->setBillboardType(Ogre::BBT_PERPENDICULAR_COMMON);
    mBillboardSet->setCommonDirection(commonVector);
    mBillboardSet->setCommonUpVector(upVector);
}

void BillboardSetComponent::SetPerpendicularSelf(const Ogre::Vector3& upVector)
{
    mBillboardSet->setBillboardType(Ogre::BBT_PERPENDICULAR_SELF);
    mBillboardSet->setCommonUpVector(upVector);
}

}
