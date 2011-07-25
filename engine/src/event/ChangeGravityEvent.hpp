
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_EVENT_CHANGEGRAVITYEVENT
#define DUCTTAPE_ENGINE_EVENT_CHANGEGRAVITYEVENT

#include <string>

#include <OgreVector3.h>

#include "Event.hpp"

namespace dt {

/**
 * Event to change global gravity.
 */
class ChangeGravityEvent : public Event {
public:
    /**
      * Advanced constructor.
      * @param direction The direction in which to change gravity.
      */
    ChangeGravityEvent(Ogre::Vector3 direction);
    const std::string GetType() const;
    std::shared_ptr<Event> Clone() const;

    /**
      * Returns the new direction for gravity.
      * @returns The new direction for gravity as a Ogre::Vector3.
      */
    Ogre::Vector3 GetDirection() const;

private:
    Ogre::Vector3 mDirection;
};

}

#endif
