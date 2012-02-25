
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

CameraComponent::CameraComponent(const QString& name)
   : Component(name) {}

void CameraComponent::OnInitialize() {
    // create the ogre context if not present
    DisplayManager::Get()->CreateOgreRoot();

    mCamera = GetNode()->GetScene()->GetSceneManager()->createCamera("camera-" + dt::Utils::ToStdString(mName));
    mCamera->setNearClipDistance(0.1);

    mZOrder = DisplayManager::Get()->GetNextZOrder();
    mViewport = DisplayManager::Get()->GetRenderWindow()->addViewport(mCamera, mZOrder, 0.0f, 0.0f, 1.0f, 1.0f); // default viewport size: full window

    // set this camera component as main camera if there is no other one
    if(DisplayManager::Get()->GetMainCamera() == nullptr) {
        DisplayManager::Get()->SetMainCamera(this);
    }
}

void CameraComponent::OnDeinitialize() {
    // reset the main camera if we were the main camera
    if(DisplayManager::Get()->GetMainCamera() == this)
        DisplayManager::Get()->SetMainCamera(nullptr);

    mCamera->getSceneManager()->destroyCamera(mCamera);
    DisplayManager::Get()->GetRenderWindow()->removeViewport(mZOrder);
}

void CameraComponent::OnEnable() {
    mCamera->setVisible(true);
}

void CameraComponent::OnDisable() {
    mCamera->setVisible(false);
}

void CameraComponent::OnUpdate(double time_diff) {
    mCamera->setPosition(mNode->GetPosition(Node::SCENE));
    mCamera->setOrientation(mNode->GetRotation(Node::SCENE));
}

Ogre::Ray CameraComponent::GetCameraToViewportRay(float x, float y) {
    return mCamera->getCameraToViewportRay(x, y);
}

void CameraComponent::LookAt(Ogre::Vector3 target_point) {
    mCamera->lookAt(target_point);
    mNode->SetRotation(mCamera->getOrientation());
}

void CameraComponent::LookAt(float x, float y, float z) {
    LookAt(Ogre::Vector3(x, y, z));
}

void CameraComponent::SetupViewport(float left, float top, float width, float height) {
    mViewport->setDimensions(left, top, width, height);
}

Ogre::Camera* CameraComponent::GetCamera() {
	return mCamera;
}

}
