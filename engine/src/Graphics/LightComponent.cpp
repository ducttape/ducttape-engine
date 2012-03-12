
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include <Graphics/LightComponent.hpp>

#include <Scene/Node.hpp>
#include <Scene/Scene.hpp>
#include <Utils/Utils.hpp>

#include <OgreSceneManager.h>

namespace dt {

LightComponent::LightComponent(const QString name)
    : Component(name),
      mLight(nullptr),
      mSceneNode(nullptr),
      mCastShadows(true) {}

void LightComponent::onInitialize() {
    mLight = getNode()->getScene()->getSceneManager()->createLight(Utils::toStdString(mName));

    // Set the point light as the default light type
    mLight->setType(Ogre::Light::LT_POINT);

    // White light
    mLight->setDiffuseColour(1.0, 1.0, 1.0);
    mLight->setSpecularColour(1.0, 1.0, 1.0);
    mLight->setDirection(Ogre::Vector3(0,0,1));

    mSceneNode = getNode()->getScene()->getSceneManager()->getRootSceneNode()->
                                        createChildSceneNode(Utils::toStdString(mName) + "-node");
    mSceneNode->attachObject(mLight);
}

void LightComponent::onDeinitialize() {
    Ogre::SceneManager* scene_mgr = getNode()->getScene()->getSceneManager();

    if(mLight != nullptr)
        scene_mgr->destroyLight(mLight);

    if(mSceneNode != nullptr)
        scene_mgr->destroySceneNode(mSceneNode);
}

void LightComponent::onEnable() {
    mLight->setVisible(true);
}

void LightComponent::onDisable() {
    mLight->setVisible(false);
}

void LightComponent::onUpdate(double time_diff) {
    mSceneNode->setPosition(getNode()->getPosition(Node::SCENE));
    mSceneNode->setOrientation(getNode()->getRotation(Node::SCENE));
    mSceneNode->setScale(getNode()->getScale(Node::SCENE));
}

void LightComponent::setColor(const Ogre::ColourValue color) {
    mLight->setDiffuseColour(color);
    mLight->setSpecularColour(color);
    emit colorChanged(color);
}

void LightComponent::setColor(float r, float g, float b, float a) {
    setColor(Ogre::ColourValue(r, g, b, a));
}

Ogre::Light* LightComponent::getOgreLight() const {
    return mLight;
}

void LightComponent::setCastShadows(bool cast_shadows) {
    mCastShadows = cast_shadows;
    if(mLight != nullptr) {
        mLight->setCastShadows(mCastShadows);
    }
}

bool LightComponent::getCastShadows() const {
    return mCastShadows;
}

}
