
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_TESTS_RANDOMTEST
#define DUCTTAPE_ENGINE_TESTS_RANDOMTEST

#include "Test.hpp"

#include <Utils/Random.hpp>

#include <iostream>

namespace RandomTest {

class RandomTest : public Test {
public:
    bool Run(int argc, char** argv);
    QString GetTestName();
};

} // namespace RandomTest

#endif
