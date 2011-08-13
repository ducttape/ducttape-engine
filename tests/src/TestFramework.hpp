
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_TESTS_TESTFRAMEWORK
#define DUCTTAPE_ENGINE_TESTS_TESTFRAMEWORK

#include "Test.hpp"

#include <QString>

#include <boost/ptr_container/ptr_map.hpp>

boost::ptr_map<QString, Test> Tests;

void AddTest(Test* test);

Test* GetTest(QString name);

int main(int argc, char** argv);

#endif
