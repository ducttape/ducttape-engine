
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include "RandomTest/RandomTest.hpp"

namespace RandomTest {

bool RandomTest::Run(int argc, char** argv) {
    dt::Random::Initialize();
    std::cout << "===== Random Int Test =====" << std::endl;
    for(int i = 0; i < 20; ++i)
        std::cout << dt::Random::Get(100, 1000) << std::endl;

    std::cout << "===== Random Float Test =====" << std::endl;
    for(int i = 0; i < 20; ++i)
        std::cout << dt::Random::Get(-516.1f,123.0f) << std::endl;
    return true;
}

QString RandomTest::GetTestName() {
    return "Random";
}

}
