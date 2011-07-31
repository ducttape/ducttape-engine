
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

#include <cstdint>
#include <string>
#include <vector>

namespace dt {

namespace Utils {

template <typename Source> DUCTTAPE_API std::string ToString(const Source& source) {
    return boost::lexical_cast<std::string>(source);
}

template <typename Source> DUCTTAPE_API int32_t ToInt(const Source& source) {
    return boost::lexical_cast<int32_t>(source);
}

std::vector<std::string> DUCTTAPE_API Split(const std::string& in, const std::string& chars);

} // namespace Utils

} // namespace dt

#endif
