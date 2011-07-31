
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include "EventListener.hpp"

namespace dt {

EventListener::~EventListener() {}

EventListener::Priority EventListener::GetEventPriority() const {
    return EventListener::NORMAL;
}

bool EventListener::SortHelper(const EventListener* l, const EventListener* r) {
    return static_cast<uint8_t>(l->GetEventPriority()) < static_cast<uint8_t>(r->GetEventPriority());
}

}
