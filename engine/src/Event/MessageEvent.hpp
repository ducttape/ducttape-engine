
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_EVENT_MESSAGEEVENT
#define DUCTTAPE_ENGINE_EVENT_MESSAGEEVENT

#include <Config.hpp>

#include <QString>

#include <Event/Event.hpp>

namespace dt {

/**
  * Event for sending generic string messages through the event system.
  */
class DUCTTAPE_API MessageEvent : public virtual Event {
public:
    /**
      * Advanced constructor.
      * @param message The message for this Event.
      */
    MessageEvent(const QString& message);
    const QString GetType() const;

    /**
      * Returns the message of this Event.
      * @returns The message of the Event.
      */
    const QString& GetMessageText() const;
protected:
    QString mMessage;   //!< The message of this Event.
};

}

#endif
