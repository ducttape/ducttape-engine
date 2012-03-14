
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_UTILS_RANDOM
#define DUCTTAPE_ENGINE_UTILS_RANDOM

#include <Config.hpp>

#include <cstdint>
#include <random>

namespace dt {

/**
  * A pseudorandom number generator helper.
  */
class DUCTTAPE_API Random {
public:
    /**
      * Initializes the generator. This uses the current time to seed the generator.
      */
    static void initialize();

    /**
      * Returns a random integer.
      * @param min The minimum return value.
      * @param max The maximum return value.
      * @returns A random integer.
      */
    static int32_t get(int32_t min, int32_t max);

    /**
      * Returns a random float.
      * @param min The minimum return value.
      * @param max The maximum return value.
      * @returns A random float.
      */
    static float get(float min, float max);
private:
    static std::mt19937 Generator;  //!< The generator used to generate pseudorandom numbers.
};

} // namespace dt

#endif
