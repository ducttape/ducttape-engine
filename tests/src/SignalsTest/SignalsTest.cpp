
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include "SignalsTest/SignalsTest.hpp"

#include <Core/Root.hpp>

#include <QObject>

namespace SignalsTest {

bool SignalsTest::Run(int argc, char** argv) {
    dt::Root& root = dt::Root::GetInstance();
    root.Initialize(argc, argv);

    TestComponent component("test");
    CallbackClass callback;
    QObject::connect(&component, SIGNAL(Invoked()),
                     &callback, SLOT(TheCallback()));
    component.Invoke();

    if(!callback.mInvoked) {
        std::cerr << "Signal callback not received correctly." << std::endl;
        return false;
    }

    root.Deinitialize();
    return true;
}

QString SignalsTest::GetTestName() {
    return "Signals";
}

////////////////////////////////////////////////////////////////

TestComponent::TestComponent(const QString& name)
    : dt::Component(name) {}

void TestComponent::OnCreate() {}

void TestComponent::OnDestroy() {}

void TestComponent::Invoke() {
    emit Invoked();
}

////////////////////////////////////////////////////////////////

CallbackClass::CallbackClass()
    : mInvoked(false) {}

void CallbackClass::TheCallback() {
    std::cout << "Callback was called." << std::endl;
    mInvoked = true;
}

} // namespace SignalsTest
