
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include <Audio/SoundsControlEvent.hpp>

namespace dt {

SoundsControlEvent::SoundsControlEvent(SoundsControlEvent::Action action)
    : mAction(action) {}

const QString SoundsControlEvent::GetType() const {
   return "DT_SOUNDSCONTROLEVENT";
}

std::shared_ptr<Event> SoundsControlEvent::Clone() const {
    std::shared_ptr<Event> ptr(new SoundsControlEvent(mAction));
    return ptr;
}

SoundsControlEvent::Action SoundsControlEvent::GetAction() const {
    return mAction;
}

}
