
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include "Event.hpp"

#include "Root.hpp"

namespace dt {

Event::Event() {}

bool Event::IsNetworkEvent() const {
    return false;
}

uint32_t Event::GetTypeID() const {
    return Root::get_mutable_instance().GetStringManager()->Get(GetType());
}

bool Event::TranslateFrom(Event* raw_event) {
    if(raw_event->GetType() == GetType()) {
        // Infinite loop != infinite fun.
        // Explanation: The event is supposed to be translated from another event of the same type.
        // The BindingsManager would receive this event - and translate it again.
        // Thus we would be stuck in an infinite loop.
        return false;
    }
    return true;
}

}
