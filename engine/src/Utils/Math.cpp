
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include <Utils/Math.hpp>

#include <cmath>
#include <cstdint>

namespace dt {

namespace Math {
    float bounceBoth(float value1, float value2, float amount, float overshoot) {
        overshoot *= 0.25949f;

        value2 -= value1;
        amount /= 0.5f;

        if(amount < 1.f) {
            return value2 / 2.f * (pow(amount, 2.f) * ((overshoot + 1.f) * amount - overshoot)) + value1;
        } else {
            amount -= 2.f;
            return value2 / 2.f * (pow(amount, 2.f) * ((overshoot + 1.f) * amount + overshoot) + 2.f) + value1;
        }
    }

    float bounceIn(float value1, float value2, float amount, float overshoot) {
        overshoot *= 0.170158f;
        value2 -= value1;

        return value2 * pow(amount, 2.f) * ((overshoot + 1.f) * amount - overshoot) + value1;
    }

    float bounceOut(float value1, float value2, float amount, float overshoot) {
        overshoot *= 0.170158f;
        amount = amount / 1.f - 1.f;
        value2 -= value1;

        return value2 * (pow(amount, 2.f) * ((overshoot + 1.f) * amount + overshoot) + 1.f) + value1;
    }

    float clamp(float value, float minval, float maxval) {
        if(value > maxval)
            value = maxval;
        else if(value < minval)
            value = minval;

        return value;
    }

    float hermite(float value1, float tangent1, float value2, float tangent2, float amount) {
        float result = 0.f;
        if(amount == 0.f)
            result = value1;
        else if(amount == 1.f)
            result = value2;
        else
            result = (2.f * value1 - 2.f * value2 + tangent2 + tangent1) * pow(amount, 3.f) + (3.f * value2 - 3.f * value1 - 2.f * tangent1 - tangent2) * pow(amount, 2.f) + tangent1 * amount + value1;

        return result;
    }

    float lerp(float value1, float value2, float amount) {
        float result = clamp(amount, 0.f, 1.f);
        return value1 + (value2 - value1) * result;
    }

    float lerp2(float value1, float value2, float amount) {
        float result = clamp(amount, 0.f, 1.f);

        if(result > 0.5f)
            return value2 + (value1 - value2) * ((result - 0.5f) * 2.f);
        else
            return value1 + (value2 - value1) * (result * 2.f);
    }

    float multiLerp(std::vector<float>& values, float amount) {
        float result = clamp(amount, 0.f, 1.f);
        if(result == 1.f)
            return *(values.end());

        int32_t position = int32_t(floor(amount * (values.size() - 1)));
        float individual_lerp = 1.f / values.size() - 1;
        float current_lerp = amount - position * individual_lerp;
        float lerp_amount = current_lerp / individual_lerp;

        return lerp(values[position], values[position + 1], lerp_amount);
    }

    float nextPow2(float value) {
        float logbase2 = log(value) / log(2.0);
        float p = pow(2, ceil(logbase2));
        return p < 0 ? ceil(p - 0.5) : floor(p + 0.5);
    }

    float smoothStep(float value1, float value2, float amount) {
        float result = clamp(amount, 0.f, 1.f);
        result = hermite(value1, 0.f, value2, 0.f, result);
        return result;
     }

    float smoothStep2(float value1, float value2, float amount) {
        float result = clamp(amount, 0.f, 1.f);

        if(result > 0.5f)
            result = hermite(value2, 0.f, value1, 0.f, (result - 0.5f) * 2);
        else
            result = hermite(value1, 0.f, value2, 0.f, result * 2);

        return result;
     }

} // namespace Math

} // namespace dt
