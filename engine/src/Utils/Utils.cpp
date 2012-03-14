
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include <Utils/Utils.hpp>

namespace dt {

namespace Utils {

std::string toStdString(const QString qstring) {
    return std::string(qstring.toLocal8Bit().data());
}

uint32_t mAutoId = 0;

uint32_t autoId() {
    mAutoId++;
    return mAutoId;
}

} // namespace Utils

} // namespace dt
