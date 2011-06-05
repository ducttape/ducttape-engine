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

}
