
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

LightComponent::LightComponent(const QString& name)
    : Component(name),
      mLight(nullptr),
      mSceneNode(nullptr),
      mCastShadows(true) {}

void LightComponent::OnColorChanged() {}

void LightComponent::OnCreate() {
    mLight = GetNode()->GetScene()->GetSceneManager()->createLight(Utils::ToStdString(mName));

    // Set the point light as the default light type
    mLight->setType(Ogre::Light::LT_POINT);

    // White light
    mLight->setDiffuseColour(1.0, 1.0, 1.0);
    mLight->setSpecularColour(1.0, 1.0, 1.0);
    mLight->setDirection(Ogre::Vector3(0,0,1));

    mSceneNode = GetNode()->GetScene()->GetSceneManager()->getRootSceneNode()->createChildSceneNode(Utils::ToStdString(mName) + "-node");
    mSceneNode->attachObject(mLight);
}

void LightComponent::OnDestroy() {
    Ogre::SceneManager* scene_mgr = GetNode()->GetScene()->GetSceneManager();

    if(mLight != nullptr)
        scene_mgr->destroyLight(mLight);

    if(mSceneNode != nullptr)
        scene_mgr->destroySceneNode(mSceneNode);
}

void LightComponent::OnEnable() {
    mLight->setVisible(true);
}

void LightComponent::OnDisable() {
    mLight->setVisible(false);
}

void LightComponent::OnUpdate(double time_diff) {
    mSceneNode->setPosition(GetNode()->GetPosition(Node::SCENE));
    mSceneNode->setOrientation(GetNode()->GetRotation(Node::SCENE));
    mSceneNode->setScale(GetNode()->GetScale(Node::SCENE));
}

void LightComponent::SetColor(const Ogre::ColourValue color) {
    mLight->setDiffuseColour(color);
    mLight->setSpecularColour(color);
    emit ColorChanged(color);
    OnColorChanged();
}

Ogre::Light* LightComponent::GetOgreLight() const {
    return mLight;
}

void LightComponent::SetCastShadows(bool cast_shadows) {
    mCastShadows = cast_shadows;
    if(mLight != nullptr) {
        mLight->setCastShadows(mCastShadows);
    }
}

bool LightComponent::GetCastShadows() const {
    return mCastShadows;
}

}
