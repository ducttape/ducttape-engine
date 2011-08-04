
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include <Event/SimpleEventBinding.hpp>

namespace dt {

SimpleEventBinding::SimpleEventBinding(Event* target, const QString& trigger_type)
    : EventBinding(target),
      mTriggerType(trigger_type) {}

bool SimpleEventBinding::MatchesEvent(std::shared_ptr<Event> e) {
    return e->GetType() == mTriggerType;
}

}
