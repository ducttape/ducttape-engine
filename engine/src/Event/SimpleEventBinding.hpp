
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_EVENT_SIMPLEEVENTBINDING
#define DUCTTAPE_ENGINE_EVENT_SIMPLEEVENTBINDING

#include <Config.hpp>

#include <Event/Event.hpp>
#include <Event/EventBinding.hpp>

#include <QString>

#include <memory>

namespace dt {

/**
  * A simple event binding that triggers when an Event
  * of a specific type occurs.
  */
class DUCTTAPE_API SimpleEventBinding : public EventBinding {
public:
    /**
      * Advanced constructor.
      * @param target The event that is being triggered.
      * @param trigger_type The type of the event that triggers this binding.
      */
    SimpleEventBinding(Event* target, const QString& trigger_type);
    bool MatchesEvent(std::shared_ptr<Event> e);

private:
    QString mTriggerType;   //!< The type of the event that triggers this binding.

};

}

#endif
