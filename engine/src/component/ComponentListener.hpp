#ifndef DUCTTAPE_ENGINE_COMPONENT_COMPONENTLISTENER
#define DUCTTAPE_ENGINE_COMPONENT_COMPONENTLISTENER

#include <string>

#include <boost/signals2.hpp>
#include <boost/ptr_container/ptr_map.hpp>

namespace dt {

/**
  * Class for handling callbacks from components in game context.
  * @see Component
  */
class ComponentListener {
public:
    /**
      * Binds a slot to a signal.
      * @param signal_identifier The name of the signal being called by the ComponentListener. Example: "Triggered" or "AnimationFinished".
      * @param slot The callback function.
      * @returns A boost signals2 connection to disconnect the slot again.
      */
    boost::signals2::connection BindSlot(const std::string& signal_identifier, boost::function<void ()> slot);

protected:
    /**
      * Calls the signal with the identifier given.
      * @param signal_identifier The signal to call.
      */
    void _CallSignal(const std::string& signal_identifier);
    boost::ptr_map<std::string, boost::signals2::signal<void ()> > mSignals;    //!< The list of signals.
};

}

#endif
