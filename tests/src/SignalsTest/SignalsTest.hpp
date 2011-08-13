
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_TESTS_SIGNALSTEST
#define DUCTTAPE_ENGINE_TESTS_SIGNALSTEST

#include "Test.hpp"

#include <Core/Root.hpp>
#include <Scene/Component.hpp>

#include <QObject>

namespace SignalsTest {

class SignalsTest : public Test {
public:
    bool Run(int argc, char** argv);
    QString GetTestName();
};

////////////////////////////////////////////////////////////////

class TestComponent : public dt::Component {
    Q_OBJECT

public:
    TestComponent(const QString& name);
    void OnCreate();
    void OnDestroy();

public slots:
    void Invoke();

signals:
    void Invoked();
};

////////////////////////////////////////////////////////////////

class CallbackClass : public QObject {
    Q_OBJECT

public:
    CallbackClass();
    bool mInvoked;
public slots:
    void TheCallback();
};

} // namespace test_signals

#endif
