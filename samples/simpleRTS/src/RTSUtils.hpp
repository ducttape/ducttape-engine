
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef RTSUTILS_HPP
#define RTSUTILS_HPP

#include <OgreVector3.h>

namespace RTSUtils {
    
    /**
     * Evaluates a 2D rotation.
     * @param point The point to evaluate.
     * @param origin The origin of the rotation.
     * @param angle The angle of the rotation. (expressed in radian).
     * @return a new point with new X and Y components,
     * while Y remains unchanged (it's the same of the param point).
     */
    Ogre::Vector3 Rotation2D(const Ogre::Vector3& point, const Ogre::Vector3& origin, float angle);
}       

#endif // RTSUTILS_HPP
