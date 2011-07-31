
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_EVENT_EVENTBINDING
#define DUCTTAPE_ENGINE_EVENT_EVENTBINDING



namespace dt {

/**
  * Binds an Event to call another, more specific event. Usually used
  * to forward raw input.
  */
class DUCTTAPE_API EventBinding {
public:
    /**
      * Advanced constructor.
      * @param target The new event to be triggered.
      */
    EventBinding(Event* target);

    /**
      * Destructor.
      */
    ~EventBinding();

    /**
      * Called when an event occurs.
      * @param e The event that occured.
      */
    void HandleEvent(std::shared_ptr<Event> e);

    /**
      * Called when an event occurs that triggered the EventBinding.
      */
    void TriggerEvent();

    /**
      * Returns whether the binding should trigger when the specified event is received.
      * @param e The event that was received.
      * @returns Whether the binding should trigger.
      */
    virtual bool MatchesEvent(std::shared_ptr<Event> e) = 0;

private:
    std::shared_ptr<Event> mTarget; //!< The new event to be triggered.
};

}

#endif
