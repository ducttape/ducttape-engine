
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include "EventBinding.hpp"

#include "Root.hpp"

namespace dt {

EventBinding::EventBinding(Event* target) {
    mTarget = std::shared_ptr<Event>(target);
}

EventBinding::~EventBinding() {}

void EventBinding::HandleEvent(std::shared_ptr<Event> e) {
    if(MatchesEvent(e))
        TriggerEvent();
}

void EventBinding::TriggerEvent() {
    std::shared_ptr<Event> target = mTarget->Clone();
    if(! MatchesEvent(target)) // prevent looping
        EventManager::Get()->InjectEvent(target);
}

}
