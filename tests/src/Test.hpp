
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_TESTS_TEST
#define DUCTTAPE_ENGINE_TESTS_TEST

#include <QString>

class Test {
public:
    virtual QString GetTestName() = 0;
    virtual bool Run(int argc, char** argv) = 0;
};

#endif
