#include "component/Component.hpp"
#include "scene/Node.hpp"

namespace dt {

Component::~Component() {}

void Component::Activate() {
    mIsActivated = true;
    OnActivate();
}

void Component::Deactivate() {
    mIsActivated = false;
    OnDeactivate();
}

bool Component::IsActivated() {
    return mIsActivated;
}

boost::signals2::connection Component::BindSlot(const std::string& signal_identifier, boost::function<void ()> slot) {
    return mSignals[signal_identifier].connect(slot);
}

void Component::_CallSignal(const std::string& signal_identifier) {
    mSignals[signal_identifier]();
}

}
