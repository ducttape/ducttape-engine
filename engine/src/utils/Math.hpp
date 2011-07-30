
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef MATH_HPP
#define MATH_HPP

#include <cmath>
#include <iostream>
#include <vector>

namespace dt {

namespace Math {
	float BounceBoth(const float value1, const float value2, const float amount, const float overshoot = 20);
	float BounceIn(const float value1, const float value2, const float amount, const float overshoot = 20);
	float BounceOut(const float value1, const float value2, const float amount, const float overshoot = 20);
	float Clamp(const float value, const float minval, const float maxval);
	float Hermite(const float value1, const float tangent1, const float value2, const float tangent2, const float amount);
	float Lerp(const float value1, const float value2, const float amount);
	float Lerp2(const float value1, const float value2, const float amount);
	float MultiLerp(const std::vector<float>& values, const float amount);
	float NextPow2(const float value);
	float SmoothStep(const float value1, const float value2, const float amount);
    float SmoothStep2(const float value1, const float value2, const float amount);

} // namespace Math

} // namespace dt

#endif
