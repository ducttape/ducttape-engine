#include "Component.hpp"

namespace dt {

Component::Component() {
    mName = "component-generated-name";
}

Component::Component(const std::string& name) {
    mName = name;
}

const std::string& Component::GetName() const {
    return mName;
}

void Update(float time_delta) {}

}
