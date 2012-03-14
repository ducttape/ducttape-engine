
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include <Graphics/CameraComponent.hpp>

#include <Graphics/DisplayManager.hpp>
#include <Scene/Node.hpp>
#include <Scene/Scene.hpp>
#include <Utils/Utils.hpp>

#include <OgreSceneManager.h>

namespace dt {

CameraComponent::CameraComponent(const QString name)
   : Component(name) {}

void CameraComponent::onInitialize() {
    // create the ogre context if not present
    DisplayManager::get()->createOgreRoot();

    mCamera = getNode()->getScene()->getSceneManager()->createCamera("camera-" + dt::Utils::toStdString(mName));
    mCamera->setNearClipDistance(0.1);

    mZOrder = DisplayManager::get()->getNextZOrder();
    mViewport = DisplayManager::get()->getRenderWindow()->addViewport(mCamera, mZOrder, 0.0f, 0.0f, 1.0f, 1.0f); // default viewport size: full window

    // set this camera component as main camera if there is no other one
    if(DisplayManager::get()->getMainCamera() == nullptr) {
        DisplayManager::get()->setMainCamera(this);
    }
}

void CameraComponent::onDeinitialize() {
    // reset the main camera if we were the main camera
    if(DisplayManager::get()->getMainCamera() == this)
        DisplayManager::get()->setMainCamera(nullptr);

    mCamera->getSceneManager()->destroyCamera(mCamera);
    DisplayManager::get()->getRenderWindow()->removeViewport(mZOrder);
}

void CameraComponent::onEnable() {
    mCamera->setVisible(true);
}

void CameraComponent::onDisable() {
    mCamera->setVisible(false);
}

void CameraComponent::onUpdate(double time_diff) {
    mCamera->setPosition(mNode->getPosition(Node::SCENE));
    mCamera->setOrientation(mNode->getRotation(Node::SCENE));
}

Ogre::Ray CameraComponent::getCameraToViewportRay(float x, float y) {
    return mCamera->getCameraToViewportRay(x, y);
}

void CameraComponent::lookAt(Ogre::Vector3 target_point) {
    mCamera->lookAt(target_point);
    mNode->setRotation(mCamera->getOrientation());
}

void CameraComponent::lookAt(float x, float y, float z) {
    lookAt(Ogre::Vector3(x, y, z));
}

void CameraComponent::setupViewport(float left, float top, float width, float height) {
    mViewport->setDimensions(left, top, width, height);
}

Ogre::Camera* CameraComponent::getCamera() {
	return mCamera;
}

}
