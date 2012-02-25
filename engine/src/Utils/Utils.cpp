
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

std::string ToStdString(const QString& qstring) {
    return std::string(qstring.toLocal8Bit().data());
}

uint32_t mAutoId = 0;

uint32_t AutoId() {
    mAutoId++;
    return mAutoId;
}

boost::uuids::uuid GenerateUUIDRandom() {
    boost::uuids::random_generator gen;
    boost::uuids::uuid uuid = gen();
    return uuid;
}

boost::uuids::uuid GenerateUUIDFromString(const QString& qstring) {
    boost::uuids::string_generator gen;
    boost::uuids::uuid uuid = gen(ToStdString(qstring));
    return uuid;
}


Ogre::Vector3 FloatsToOgreVector3(const float* float_vector) { 
    Ogre::Vector3 ogre_vector;
    ogre_vector.x = float_vector[0];
    ogre_vector.y = float_vector[1];
    ogre_vector.z = float_vector[2];
    return ogre_vector;
}

void OgreVector3ToFloats(const Ogre::Vector3& ogre_vector, float* float_vector) {
    float_vector[0] = ogre_vector.x;
    float_vector[1] = ogre_vector.y;
    float_vector[2] = ogre_vector.z;
}


} // namespace Utils

} // namespace dt
