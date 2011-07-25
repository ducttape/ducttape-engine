
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_FREEZEPHYSICSEVENT
#define DUCTTAPE_ENGINE_FREEZEPHYSICSEVENT

#include <string>

#include "Event.hpp"

namespace dt {

/**
 * Event to freeze all physics.
 */
class FreezePhysicsEvent : public Event {
public:
    /**
     * Default constructor.
     */
    FreezePhysicsEvent();
    const std::string GetType() const;
    std::shared_ptr<Event> Clone() const;
};

}

#endif
