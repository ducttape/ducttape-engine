
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_TESTS_SIGNALSTEST
#define DUCTTAPE_ENGINE_TESTS_SIGNALSTEST

#include <Config.hpp>

#include "Test.hpp"

#include <Core/Root.hpp>
#include <Scene/Component.hpp>

#include <QObject>

namespace SignalsTest {

class SignalsTest : public Test {
public:
    bool run(int argc, char** argv);
    QString getTestName();
};

////////////////////////////////////////////////////////////////

class TestComponent : public dt::Component {
    Q_OBJECT

public:
    TestComponent(const QString name);
    void onInitialize();
    void onDeinitialize();

public slots:
    void invoke();

signals:
    void invoked();
};

////////////////////////////////////////////////////////////////

class CallbackClass : public QObject {
    Q_OBJECT

public:
    CallbackClass();
    bool mInvoked;
public slots:
    void theCallback();
};

} // namespace test_signals

#endif
