
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_TESTS_QOBJECTTEST
#define DUCTTAPE_ENGINE_TESTS_QOBJECTTEST

#include "Test.hpp"

#include <Core/Root.hpp>
#include <Scene/Component.hpp>
#include <Logic/TriggerComponent.hpp>

#include <QVariant>
#include <cstdlib>

namespace QObjectTest {

class QObjectTest : public Test {
public:
    bool Run(int argc, char** argv);
    QString GetTestName();
};

} // namespace QObjectTest

#endif
