
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include "Random.hpp"

namespace dt {

void Random::Initialize() {
    Generator.seed(time(0));
}

int Random::Get(int min, int max) {
    return std::uniform_int_distribution<int>(min, max)(Generator);
}

float Random::Get(float min, float max) {
    return std::uniform_real_distribution<float>(min, max)(Generator);
}

std::mt19937 Random::Generator;

}
