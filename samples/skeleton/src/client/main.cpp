#include <iostream>

#include "Root.hpp"

int main() {
    dt::Root::get_mutable_instance().Initialize();

    std::cout << "Hello World." << std::endl;

    dt::Root::get_mutable_instance().Deinitialize();
    return 0;
}
