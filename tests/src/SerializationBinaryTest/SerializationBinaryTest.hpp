
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_TESTS_SERIALIZATIONBINARYTEST
#define DUCTTAPE_ENGINE_TESTS_SERIALIZATIONBINARYTEST

#include <Config.hpp>

#include "Test.hpp"

#include <Core/Root.hpp>
#include <Utils/Logger.hpp>

namespace SerializationBinaryTest {

class SerializationBinaryTest : public Test {
public:
    bool Run(int argc, char** argv);
    QString GetTestName();
};

}

#endif
