
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef MATH_HPP
#define MATH_HPP

#include <Config.hpp>

#include <vector>

namespace dt {

namespace Math {
    float DUCTTAPE_API bounceBoth(const float value1, const float value2, const float amount, const float overshoot = 20);
    float DUCTTAPE_API bounceIn(const float value1, const float value2, const float amount, const float overshoot = 20);
    float DUCTTAPE_API bounceOut(const float value1, const float value2, const float amount, const float overshoot = 20);
    float DUCTTAPE_API clamp(const float value, const float minval, const float maxval);
    float DUCTTAPE_API hermite(const float value1, const float tangent1, const float value2, const float tangent2, const float amount);
    float DUCTTAPE_API lerp(const float value1, const float value2, const float amount);
    float DUCTTAPE_API lerp2(const float value1, const float value2, const float amount);
    float DUCTTAPE_API multiLerp(const std::vector<float>& values, const float amount);
    float DUCTTAPE_API nextPow2(const float value);
    float DUCTTAPE_API smoothStep(const float value1, const float value2, const float amount);
    float DUCTTAPE_API smoothStep2(const float value1, const float value2, const float amount);
} // namespace Math

} // namespace dt

#endif
