
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include "PhysicsBodyComponent.hpp"

namespace dt {

PhysicsBodyComponent::PhysicsBodyComponent(const std::string& name)
    : Component(name) {
}

void PhysicsBodyComponent::HandleEvent(std::shared_ptr<Event> e) {
    if(e->GetType() == "DT_FREEZEPHYSICS") {}
    if(e->GetType() == "DT_UNFREEZEPHYSICS") {}
    if(e->GetType() == "DT_CHANGEGRAVITY") {
        std::shared_ptr<ChangeGravityEvent> g =
            std::dynamic_pointer_cast<ChangeGravityEvent>(e);
        g->GetDirection();
    }
}

void PhysicsBodyComponent::OnCreate() {}

void PhysicsBodyComponent::OnDestroy() {}

void PhysicsBodyComponent::OnUpdate(double time_diff) {
}

}
