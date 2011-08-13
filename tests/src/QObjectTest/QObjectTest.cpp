
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include "QObjectTest/QObjectTest.hpp"

namespace QObjectTest {

bool QObjectTest::Run(int argc, char** argv) {
    // Don't initialize Root, we don't need the StringManager.
    // We gave that Component a name. Components love names.

    dt::TriggerComponent trigger("testcomponent");
    QObject *object = &trigger;
    QVariant var = object->property("name");
    char* c = var.toString().toAscii().data();

    if(strcmp(c, "testcomponent") != 0) {
        std::cerr << "Could not get property [mName] of TriggerComponent. Got \"" << c << "\" instead of \"testcomponent\"." << std::endl;
        return false;
    }
    return true;
}

QString QObjectTest::GetTestName() {
    return "QObject";
}

}
