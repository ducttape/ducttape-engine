#include "ComponentListener.hpp"

namespace dt {

boost::signals2::connection ComponentListener::BindSlot(const std::string& signal_identifier, boost::function<void ()> slot) {
    return mSignals[signal_identifier].connect(slot);
}

void ComponentListener::_CallSignal(const std::string& signal_identifier) {
    mSignals[signal_identifier]();
}

}
