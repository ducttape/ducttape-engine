
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_EVENT_SOUNDSCONTROLEVENT
#define DUCTTAPE_ENGINE_EVENT_SOUNDSCONTROLEVENT

#include <Config.hpp>

#include <Event/Event.hpp>

#include <QString>

#include <memory>

namespace dt {

/**
 * Event being sent when sending a play to all sounds.
 */
class DUCTTAPE_API SoundsControlEvent : public Event {
public:
    /**
      * The Action to perform with all SoundComponents.
      */
    enum Action {
        PLAY = 0,   //!< Play the sound.
        PAUSE = 1,  //!< Pause the sound.
        STOP = 2    //!< Stop the sound.
    };

    /**
     * Advanced constructor.
     * @param action The action to perform with all sounds.
     */
    SoundsControlEvent(Action action);
    const QString GetType() const;
    std::shared_ptr<Event> Clone() const;

    /**
      * Returns the Action to perform.
      * @returns The Action to perform.
      */
    Action GetAction() const;

private:
    Action mAction; //!< The Action to perform.
};

}

#endif
