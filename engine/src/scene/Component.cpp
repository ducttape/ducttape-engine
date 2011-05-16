#include "Component.hpp"

namespace dt {

Component::Component(const std::string& name) {
    mName = name;
}

std::string Component::GetName() {
    return mName;
}

}
