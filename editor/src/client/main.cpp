#include <iostream>

#include "Root.hpp"

int main() {
    dt::Root::get_mutable_instance().GetLogManager()->GetLogger().Info("Hello world!");

    return 0;
}
