
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_EVENT_BEGINFRAMEEVENT
#define DUCTTAPE_ENGINE_EVENT_BEGINFRAMEEVENT

#include <Config.hpp>

#include <Event/Event.hpp>

#include <QString>

#include <memory>

namespace dt {

/**
  * Event that is being triggered at the beginning of each frame, carrying the frame time delta.
  */
class DUCTTAPE_API BeginFrameEvent : public Event {
public:
    /**
      * Advanced constructor.
      * @param frame_time The time delta of the current frame.
      */
    BeginFrameEvent(double frame_time);
    const QString GetType() const;
    std::shared_ptr<Event> Clone() const;


    /**
      * Returns the time delta of the current frame.
      * @returns The time delta of the current frame, in seconds.
      */
    double GetFrameTime() const;

private:
    double mFrameTime;       //!< The time delta of the current frame, in seconds.

};

}

#endif
