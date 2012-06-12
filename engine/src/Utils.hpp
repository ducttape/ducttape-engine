
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_UTILS_UTILS
#define DUCTTAPE_ENGINE_UTILS_UTILS

#include <Config.hpp>

#include <QString>
#include <QUuid>

#include <iostream>
#include <sstream>
#include <cstdint>

namespace dt {

namespace Utils {

template <typename Source> QString toString(const Source& source) {
    std::stringstream temp_stream;
    temp_stream << source;
    return QString(temp_stream.str().c_str());
}

/**
  * Convert a QString to a std::string. Use it to replace qt's toStdString() which has a locale bug.
  * @param qstring The QString you are going to convert.
  * @returns The converted std::string.
  */
DUCTTAPE_API std::string toStdString(const QString qstring);

/**
  * Convert a QString to a std::wstring. Use it to replace qt's toStdWString() which has a locale bug.
  * @param qstring The QString you are going to convert.
  * @returns The converted std::wstring.
  */
DUCTTAPE_API std::wstring toWString(const QString qstring);

extern uint32_t mAutoId;

/**
  * A tool for assigning Id's
  * @returns the new id
  */
uint32_t autoId();

} // namespace Utils

} // namespace dt

#endif
