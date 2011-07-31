
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef MATH_HPP
#define MATH_HPP

#include <Config.hpp>

namespace dt {

namespace Math {
	float DUCTTAPE_API BounceBoth(const float value1, const float value2, const float amount, const float overshoot = 20);
	float DUCTTAPE_API BounceIn(const float value1, const float value2, const float amount, const float overshoot = 20);
	float DUCTTAPE_API BounceOut(const float value1, const float value2, const float amount, const float overshoot = 20);
	float DUCTTAPE_API Clamp(const float value, const float minval, const float maxval);
	float DUCTTAPE_API Hermite(const float value1, const float tangent1, const float value2, const float tangent2, const float amount);
	float DUCTTAPE_API Lerp(const float value1, const float value2, const float amount);
	float DUCTTAPE_API Lerp2(const float value1, const float value2, const float amount);
	float DUCTTAPE_API MultiLerp(const std::vector<float>& values, const float amount);
	float DUCTTAPE_API NextPow2(const float value);
	float DUCTTAPE_API SmoothStep(const float value1, const float value2, const float amount);
    float DUCTTAPE_API SmoothStep2(const float value1, const float value2, const float amount);
} // namespace Math

} // namespace dt

#endif
