#include "component/CameraComponent.hpp"
#include "Root.hpp"
#include "scene/Node.hpp"
#include "scene/Scene.hpp"

namespace dt {

CameraComponent::CameraComponent(const std::string& name, CameraComponentListener* custom_listener)
   : Component<CameraComponentListener>(name, custom_listener) {
}

void CameraComponent::HandleEvent(Event* e) {
}

void CameraComponent::OnActivate() {
    mCamera = GetNode()->GetScene()->GetSceneManager()->createCamera("camera-" + mName);
    Root::get_mutable_instance().GetDisplayManager()->RegisterCamera(this);
    Root::get_mutable_instance().GetDisplayManager()->ActivateCamera(mName);
}

void CameraComponent::OnDeactivate() {
    Root::get_mutable_instance().GetDisplayManager()->UnregisterCamera(this);
    mCamera->getSceneManager()->destroyCamera(mCamera);
}

void CameraComponent::OnUpdate() {
    mCamera->setPosition(mNode->GetPosition(Node::SCENE));
    mCamera->setOrientation(mNode->GetRotation(Node::SCENE));
}

}
