
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_UTILS_UTILS
#define DUCTTAPE_ENGINE_UTILS_UTILS

#include <Config.hpp>

#include <OgreVector3.h>

#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>

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

extern uint32_t mAutoId;

/**
  * A tool for assigning Id's
  * @returns the new id
  */
uint32_t AutoId();

/**
  * Generate a random uuid.
  * @returns the new uuid.
  */
DUCTTAPE_API boost::uuids::uuid GenerateUUIDRandom();

/**
  * Generate a uuid from a given string.
  * @returns the new uuid.
  */
DUCTTAPE_API boost::uuids::uuid GenerateUUIDFromString(const QString& qstring);

/**
 * Convert an array of 3 floats to a Ogre::Vector3
 * @param vector an array of 3 floats.
 * @return an ogre vector.
 */
Ogre::Vector3 FloatsToOgreVector3(const float* float_vector);
    
/**
 * Convert an Ogre::Vector3 to an array of 3 floats.
 * @param ogre_vector an ogre vector.
 * @param float_vector an array of 3 floats. warning: the float memory must be writable.
*/
void OgreVector3ToFloats(const Ogre::Vector3& ogre_vector, float* float_vector);

} // namespace Utils

} // namespace dt

#endif
