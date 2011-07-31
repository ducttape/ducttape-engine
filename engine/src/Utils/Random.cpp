
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include <Utils/Random.hpp>

#include <cstdint>
#include <random>

namespace dt {

void Random::Initialize() {
    Generator.seed(time(0));
}

int32_t Random::Get(int32_t min, int32_t max) {
    return std::uniform_int_distribution<int32_t>(min, max)(Generator);
}

float Random::Get(float min, float max) {
    return std::uniform_real_distribution<float>(min, max)(Generator);
}

std::mt19937 Random::Generator;

} // namespace dt
