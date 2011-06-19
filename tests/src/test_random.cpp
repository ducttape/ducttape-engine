#include <iostream>

#include "utils/Random.hpp"

int main(int argc, char** argv) {
    dt::Random::Initialize();
    std::cout << "===== Random Int Test =====" << std::endl;
    for(int i = 0; i < 20; ++i)
        std::cout << dt::Random::Get(100, 1000) << std::endl;

    std::cout << "===== Random Float Test =====" << std::endl;
    for(int i = 0; i < 20; ++i)
        std::cout << dt::Random::Get(-516.1f,123.0f) << std::endl;
    return 0;
}
