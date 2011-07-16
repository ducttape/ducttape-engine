#include "SoundsControlEvent.hpp"

namespace dt {

SoundsControlEvent::SoundsControlEvent(SoundsControlEvent::Action action) {
    mAction = action;
}

const std::string SoundsControlEvent::GetType() const {
   return "DT_SOUNDSCONTROLEVENT";
}

Event* SoundsControlEvent::Clone() const {
    return new SoundsControlEvent(mAction);
}

SoundsControlEvent::Action SoundsControlEvent::GetAction() const {
    return mAction;
}

}
