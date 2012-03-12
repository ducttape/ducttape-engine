
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include <Utils/Utils.hpp>

#include <boost/algorithm/string.hpp>
#include <boost/uuid/uuid_io.hpp>

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

boost::uuids::uuid generateUUIDRandom() {
    boost::uuids::random_generator gen;
    boost::uuids::uuid uuid = gen();
    return uuid;
}

boost::uuids::uuid generateUUIDFromString(const QString qstring) {
    boost::uuids::string_generator gen;
    boost::uuids::uuid uuid = gen(toStdString(qstring));
    return uuid;
}

} // namespace Utils

} // namespace dt
