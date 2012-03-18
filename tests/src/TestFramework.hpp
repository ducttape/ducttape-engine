
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_TESTS_TESTFRAMEWORK
#define DUCTTAPE_ENGINE_TESTS_TESTFRAMEWORK

#include <Config.hpp>

#include "Test.hpp"

#include <QString>
#include <memory>
#include <map>

typedef std::shared_ptr<Test> TestSP;

std::map<QString, TestSP> Tests;

void addTest(Test* test);

TestSP getTest(QString name);

int main(int argc, char** argv);

#endif
