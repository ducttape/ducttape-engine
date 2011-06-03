#ifndef DUCTTAPE_ENGINE_UTILS_RANDOM
#define DUCTTAPE_ENGINE_UTILS_RANDOM

#include <ctime>
#include <random>

namespace dt {

class Random {
public:
    static void Initialize();

    static int Get(int min, int max);

    static float Get(float min, float max);
private:
    static std::mt19937 Generator;
};

}

#endif
