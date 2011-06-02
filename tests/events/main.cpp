#include "Root.hpp"

#include "utils/Logger.hpp"

int main() {
    dt::Root::get_mutable_instance().Initialize();

    dt::Logger::Get().Info("Well here we are again");
    dt::Logger::Get().Debug("It's always such a pleasure");

    dt::Root::get_mutable_instance().Deinitialize();
    return 0;
}
