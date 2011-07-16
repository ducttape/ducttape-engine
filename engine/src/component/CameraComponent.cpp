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

void CameraComponent::OnActivate() {
    mCamera = GetNode()->GetScene()->GetSceneManager()->createCamera("camera-" + mName);
    Root::get_mutable_instance().GetDisplayManager()->RegisterCamera(this);
    Root::get_mutable_instance().GetDisplayManager()->ActivateCamera(mName);

    mViewport = Root::get_mutable_instance().GetDisplayManager()->GetRenderWindow()->addViewport(mCamera);
    mViewport->setBackgroundColour(Ogre::ColourValue(0, 0, 0));
    mCamera->setAspectRatio(Ogre::Real(mViewport->getActualWidth()) / Ogre::Real(mViewport->getActualHeight()));
    mCamera->setNearClipDistance(0.1);
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

}
