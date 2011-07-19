
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include "EventBinding.hpp"

#include "Root.hpp"

namespace dt {

EventBinding::EventBinding() {
    mTriggerType = "";
}

EventBinding::EventBinding(const std::string& trigger_type, Event* target) {
    if(trigger_type == target->GetType()) {
        Logger::Get().Error("The EventBinding " + trigger_type + " -> " + target->GetType() + " will cause an infinte event loop.");
    }
    mTriggerType = trigger_type;
    mTarget = std::shared_ptr<Event>(target);
}

EventBinding::~EventBinding() {}

void EventBinding::TriggerEvent(std::shared_ptr<Event> e) {
    if(e->GetType() == mTriggerType) {
        std::shared_ptr<Event> target = mTarget->Clone();
        if(target->GetType() != e->GetType()) // prevent looping
            Root::get_mutable_instance().GetEventManager()->InjectEvent(target);
    }
}

}
