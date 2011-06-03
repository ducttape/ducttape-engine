#include "TriggerComponentListener.hpp"

namespace dt {

void TriggerComponentListener::Triggered() {
    _CallSignal("Triggered");
    OnTriggered();
}

void TriggerComponentListener::OnTriggered() {}

}
