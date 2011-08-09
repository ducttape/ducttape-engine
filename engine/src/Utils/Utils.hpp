
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_UTILS_UTILS
#define DUCTTAPE_ENGINE_UTILS_UTILS

#include <Config.hpp>

#include <boost/lexical_cast.hpp>

#include <QString>

#include <cstdint>
#include <vector>

namespace dt {

namespace Utils {

template <typename Source> QString ToString(const Source& source) {
    return QString::fromStdString(boost::lexical_cast<std::string>(source));
}

} // namespace Utils

} // namespace dt

#endif
