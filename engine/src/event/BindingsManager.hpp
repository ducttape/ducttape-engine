#ifndef DUCTTAPE_ENGINE_EVENT_BINDINGSMANAGER
#define DUCTTAPE_ENGINE_EVENT_BINDINGSMANAGER

#include <boost/ptr_container/ptr_map.hpp>

#include "EventListener.hpp"
#include "EventBinding.hpp"

namespace dt {

class BindingsManager {
public:
    BindingsManager();
    ~BindingsManager();

    void Initialize();
    void Deinitialize();

    void HandleEvent(Event* e);

    uint32_t Bind(EventBinding* binding);
    void Unbind(uint32_t binding_id);
private:
    uint32_t _GetNewId();

    boost::ptr_map<uint32_t, EventBinding> mBindings;
};

}

#endif
