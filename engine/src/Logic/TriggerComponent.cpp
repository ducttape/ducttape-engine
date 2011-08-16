
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include <Logic/TriggerComponent.hpp>

namespace dt {

TriggerComponent::TriggerComponent(const QString& name, const QString& mesh_handle_d)
    : Component(name, mesh_handle_d, "debug-shape-of-TriggerComponent-" + name) {
}

void TriggerComponent::HandleEvent(std::shared_ptr<Event> e) {
    if(e->GetType() == "trigger") {

    }
}

void TriggerComponent::OnCreate() {}

void TriggerComponent::OnDestroy() {}

void TriggerComponent::OnUpdate(double time_diff) {}

}
