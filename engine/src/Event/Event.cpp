
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include <Event/Event.hpp>

#include <Core/StringManager.hpp>

namespace dt {

Event::Event()
    : mIsCanceled(false) {}

bool Event::IsNetworkEvent() const {
    return false;
}

uint32_t Event::GetTypeID() const {
    return StringManager::Get()->GetId(GetType());
}

void Event::Cancel() {
    mIsCanceled = true;
}

void Event::Uncancel() {
    mIsCanceled = false;
}

bool Event::IsCanceled() {
    return mIsCanceled;
}

}
