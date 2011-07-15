#include "component/CameraComponent.hpp"
#include "Root.hpp"
#include "scene/Node.hpp"
#include "scene/Scene.hpp"

namespace dt {

CameraComponent::CameraComponent(const std::string& name, CameraComponentListener* custom_listener)
   : Component(name, custom_listener) {
}

void CameraComponent::HandleEvent(Event* e) {
}

void CameraComponent::OnActivate() {
    mCamera = GetNode()->GetScene()->GetSceneManager()->createCamera("camera-" + mName);
    Root::get_mutable_instance().GetDisplayManager()->RegisterCamera(this);

    /*mViewport = Root::get_mutable_instance().GetDisplayManager()->GetRenderWindow()->addViewport(mCamera, rand());
    mViewport->setBackgroundColour(Ogre::ColourValue(0, 0, 0));*/
    //Ogre::Viewport* mViewport = Root::get_mutable_instance().GetDisplayManager()->GetViewport();
    //mCamera->setAspectRatio(Ogre::Real(mViewport->getActualWidth()) / Ogre::Real(mViewport->getActualHeight()));
    mCamera->setNearClipDistance(0.1);

    //Root::get_mutable_instance().GetDisplayManager()->ActivateCamera(mName);
}

void CameraComponent::OnDeactivate() {
    mCamera->getSceneManager()->destroyCamera(mCamera);
}

void CameraComponent::LookAt(Ogre::Vector3 target_point) {
    mCamera->lookAt(target_point);
    mNode->SetRotation(mCamera->getOrientation());
}

void CameraComponent::OnUpdate(float time_diff) {
    mCamera->setPosition(mNode->GetPosition(Node::SCENE));
    mCamera->setOrientation(mNode->GetRotation(Node::SCENE));
}

Ogre::Camera* CameraComponent::GetCamera() {
	return mCamera;
}

}
