
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

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
