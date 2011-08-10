
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_TESTS_CONNECTIONSTEST
#define DUCTTAPE_ENGINE_TESTS_CONNECTIONSTEST

#include "Test.hpp"

#include <Core/Root.hpp>
#include <Network/ConnectionsManager.hpp>
#include <Utils/Random.hpp>
#include <Utils/Utils.hpp>

#include <cstdint>
#include <iostream>
#include <map>
#include <memory>
#include <random>

namespace ConnectionsTest {

class ConnectionsTest : public Test {
    bool Run(int argc, char** argv);
    QString GetTestName();
};

} // namespace test_connections

#endif
