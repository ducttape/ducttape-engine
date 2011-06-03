#ifndef DUCTTAPE_ENGINE_COMPONENT_COMPONENTLISTENER
#define DUCTTAPE_ENGINE_COMPONENT_COMPONENTLISTENER

#include <string>

#include <boost/signals2.hpp>
#include <boost/ptr_container/ptr_map.hpp>

namespace dt {

class ComponentListener {
public:
    boost::signals2::connection BindSlot(const std::string& signal_identifier, boost::function<void ()> slot);

protected:
    void _CallSignal(const std::string& signal_identifier);
    boost::ptr_map<std::string, boost::signals2::signal<void ()> > mSignals;
};

}

#endif
