
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include <Audio/MusicControlEvent.hpp>

namespace dt {

MusicControlEvent::MusicControlEvent(MusicControlEvent::Action action)
    : mAction(action) {}

const QString MusicControlEvent::GetType() const {
   return "DT_MUSICCONTROLEVENT";
}

std::shared_ptr<Event> MusicControlEvent::Clone() const {
    std::shared_ptr<Event> ptr(new MusicControlEvent(mAction));
    return ptr;
}

MusicControlEvent::Action MusicControlEvent::GetAction() const {
    return mAction;
}

}
