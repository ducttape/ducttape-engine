
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_EVENT_MUSICCONTROLEVENT
#define DUCTTAPE_ENGINE_EVENT_MUSICCONTROLEVENT

#include <Config.hpp>

#include <Event/Event.hpp>

#include <memory>

namespace dt {

/**
 * Event for controlling all music components at once.
 */
class DUCTTAPE_API MusicControlEvent : public Event {
public:
    /**
      * The Action to perform with all MusicComponents.
      */
    enum Action {
        PLAY = 0,   //!< Play the music.
        PAUSE = 1,  //!< Pause the music.
        STOP = 2    //!< Stop the music.
    };

    /**
     * Advanced constructor.
     * @param action The action to perform with all sounds.
     */
    MusicControlEvent(Action action);
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
