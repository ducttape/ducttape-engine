
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include <Config.hpp>

#include <iostream>

#include "Root.hpp"

int main(int argc, char** argv) {
    dt::Root::GetInstance().Initialize(argc, argv);

    std::cout << "Hello World." << std::endl;

    dt::Root::GetInstance().Deinitialize();
    return 0;
}
