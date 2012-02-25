
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include "RTSUtils.hpp"

namespace RTSUtils {
    
Ogre::Vector3 Rotation2D(const Ogre::Vector3& point, const Ogre::Vector3& origin, float angle) {
    Ogre::Vector3 result, tmp;
    tmp = origin - point;
    result.x = tmp.x * Ogre::Math::Cos(angle) - tmp.z * Ogre::Math::Sin(angle);
    result.z = tmp.x * Ogre::Math::Sin(angle) + tmp.z * Ogre::Math::Cos(angle);
    result.x += origin.x;
    result.z += origin.z;
    result.y = point.y;
    return result;
}
    
} // namespace RTSUtils