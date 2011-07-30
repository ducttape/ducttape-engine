#include "SimpleEventBinding.hpp"

namespace dt {

SimpleEventBinding::SimpleEventBinding(Event* target, const std::string& trigger_type)
    : EventBinding(target),
      mTriggerType(trigger_type) {}

bool SimpleEventBinding::MatchesEvent(std::shared_ptr<Event> e) {
    return e->GetType() == mTriggerType;
}

}
