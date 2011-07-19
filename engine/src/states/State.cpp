
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include "State.hpp"

#include "Root.hpp"

namespace dt {

State::State() {}

uint32_t State::GetTypeID() {
    return StringManager::Get()->GetId(GetType());
}

}
