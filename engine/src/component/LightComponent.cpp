
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include "component/LightComponent.hpp"
#include "Root.hpp"
#include "scene/Node.hpp"
#include "scene/Scene.hpp"

namespace dt {

LightComponent::LightComponent(const std::string& name)
   : Component(name) {
}

void LightComponent::HandleEvent(std::shared_ptr<Event> e) {
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
