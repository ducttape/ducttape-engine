#include "ChangeGravityEvent.hpp"

namespace dt {

ChangeGravityEvent::ChangeGravityEvent(Ogre::Vector3 direction) {
    mDirection = direction;
}

const std::string ChangeGravityEvent::GetType() const {
    return "DT_CHANGEGRAVITY";
}

std::shared_ptr<Event> ChangeGravityEvent::Clone() const {
    std::shared_ptr<Event> ptr(new ChangeGravityEvent(mDirection));
    return ptr;
}

Ogre::Vector3 ChangeGravityEvent::GetDirection() const {
    return mDirection;
}

}
