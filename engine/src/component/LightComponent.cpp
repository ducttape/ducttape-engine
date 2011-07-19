
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
    LightComponent::LightComponent(const std::string& name) : Component(name) {
        mLight = nullptr;
    }

    void LightComponent::HandleEvent(std::shared_ptr<Event> e) {
    }

    void LightComponent::OnChangeColor() {
    }

    void LightComponent::OnCreate() {
        mLight = GetNode()->GetScene()->GetSceneManager()->createLight(mName);

        // Set the point light as the default light type
        mLight->setType(Ogre::Light::LT_POINT);

        // White light
        mLight->setDiffuseColour(1.0, 1.0, 1.0);
        mLight->setSpecularColour(1.0, 1.0, 1.0);
    }

    void LightComponent::OnDestroy() {
        GetNode()->GetScene()->GetSceneManager()->destroyLight(mLight);
    }

    void LightComponent::OnUpdate(double time_diff) {
        mLight->setPosition(mNode->GetPosition(Node::SCENE));
    }

    void LightComponent::SetColor(const Ogre::ColourValue color) {
        mLight->setDiffuseColour(color);
        mLight->setSpecularColour(color);
        OnChangeColor();
        _CallSignal("ChangeColor");
    }

    void LightComponent::OnEnable() {
        mLight->setVisible(true);
    }

    void LightComponent::OnDisable() {
        mLight->setVisible(false);
    }
}
