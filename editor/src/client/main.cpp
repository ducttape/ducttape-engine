#include <iostream>

#include "Root.hpp"

int main() {
    dt::Root::get_mutable_instance().Initialize();

    dt::Logger::Get().Info("Hello World");

    // create test state
    // create test event

    dt::Root::get_mutable_instance().Deinitialize();
    return 0;
}
