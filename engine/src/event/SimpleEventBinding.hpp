#ifndef DUCTTAPE_ENGINE_EVENT_SIMPLEEVENTBINDING
#define DUCTTAPE_ENGINE_EVENT_SIMPLEEVENTBINDING

#include "Config.hpp"
#include "EventBinding.hpp"
#include "utils/StringManager.hpp"

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
    SimpleEventBinding(Event* target, const std::string& trigger_type);
    bool MatchesEvent(std::shared_ptr<Event> e);

private:
    std::string mTriggerType;

};

}

#endif
