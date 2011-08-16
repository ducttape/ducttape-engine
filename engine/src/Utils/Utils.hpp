
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
#include <string>

namespace dt {

namespace Utils {

template <typename Source> QString ToString(const Source& source) {
    return QString((boost::lexical_cast<std::string>(source)).c_str());
}

/**
  * Convert a QString to a std::string. Use it to replace qt's toStdString() which has a locale bug.
  * @param qstring The QString you are going to convert.
  * @returns The converted std::string.
  */
DUCTTAPE_API std::string ToStdString(const QString& qstring);

} // namespace Utils

} // namespace dt

#endif
