#include <iostream>

#include "Root.hpp"

int main(int argc, char** argv) {
    dt::Root::get_mutable_instance().Initialize(argc, argv);

    std::cout << "Hello World." << std::endl;

    dt::Root::get_mutable_instance().Deinitialize();
    return 0;
}
