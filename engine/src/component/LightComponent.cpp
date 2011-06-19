#include "component/LightComponent.hpp"
#include "Root.hpp"
#include "scene/Node.hpp"
#include "scene/Scene.hpp"

namespace dt {

LightComponent::LightComponent(const std::string& name, LightComponentListener* custom_listener)
   : Component(name, custom_listener) {
}

void LightComponent::HandleEvent(Event* e) {
}

void LightComponent::OnActivate() {
    mLight = GetNode()->GetScene()->GetSceneManager()->createLight(mName);
    mLight->setType(Ogre::Light::LT_POINT);
    mLight->setDiffuseColour(1.0, 0.0, 0.0);
    mLight->setSpecularColour(1.0, 0.0, 0.0);
}

void LightComponent::OnDeactivate() {
    GetNode()->GetScene()->GetSceneManager()->destroyLight(mLight);
}

void LightComponent::OnUpdate(float time_diff) {
    mLight->setPosition(mNode->GetPosition(Node::SCENE));
}

}
