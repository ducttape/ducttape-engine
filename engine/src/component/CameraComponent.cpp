
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include "component/CameraComponent.hpp"
#include "Root.hpp"
#include "scene/Node.hpp"
#include "scene/Scene.hpp"

namespace dt {

CameraComponent::CameraComponent(const std::string& name)
   : Component(name) {
}

void CameraComponent::HandleEvent(Event* e) {
}

void CameraComponent::OnCreate() {
    mCamera = GetNode()->GetScene()->GetSceneManager()->createCamera("camera-" + mName);
    Root::get_mutable_instance().GetDisplayManager()->RegisterCamera(this);
    mCamera->setNearClipDistance(0.1);
}

void CameraComponent::OnDestroy() {
    mCamera->getSceneManager()->destroyCamera(mCamera);
}

void CameraComponent::LookAt(Ogre::Vector3 target_point) {
    mCamera->lookAt(target_point);
    mNode->SetRotation(mCamera->getOrientation());
}

void CameraComponent::OnUpdate(double time_diff) {
    mCamera->setPosition(mNode->GetPosition(Node::SCENE));
    mCamera->setOrientation(mNode->GetRotation(Node::SCENE));
}

Ogre::Camera* CameraComponent::GetCamera() {
	return mCamera;
}

}
