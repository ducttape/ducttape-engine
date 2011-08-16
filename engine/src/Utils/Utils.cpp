
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include <Utils/Utils.hpp>

#include <boost/algorithm/string.hpp>

namespace dt {

namespace Utils {

    std::string ToStdString(const QString& qstring) {
        return std::string(qstring.toLocal8Bit().data());
    }

} // namespace Utils

} // namespace dt
