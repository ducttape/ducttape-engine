#include "component/LightComponent.hpp"
#include "Root.hpp"
#include "scene/Node.hpp"
#include "scene/Scene.hpp"

namespace dt {

LightComponent::LightComponent(const std::string& name)
   : Component(name) {
}

void LightComponent::HandleEvent(Event* e) {
}

void LightComponent::OnCreate() {
    mLight = GetNode()->GetScene()->GetSceneManager()->createLight(mName);
    mLight->setType(Ogre::Light::LT_POINT);
    mLight->setDiffuseColour(1.0, 0.0, 0.0);
    mLight->setSpecularColour(1.0, 0.0, 0.0);
}

void LightComponent::OnDestroy() {
    GetNode()->GetScene()->GetSceneManager()->destroyLight(mLight);
}

void LightComponent::OnUpdate(float time_diff) {
    mLight->setPosition(mNode->GetPosition(Node::SCENE));
}

}
