
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include "SignalsTest/SignalsTest.hpp"

#include <Core/Root.hpp>

#include <QObject>

#include <iostream>

namespace SignalsTest {

bool SignalsTest::run(int argc, char** argv) {
    dt::Root& root = dt::Root::getInstance();
    root.initialize(argc, argv);

    TestComponent component("test");
    CallbackClass callback;
    QObject::connect(&component, SIGNAL(invoked()),
                     &callback,  SLOT(theCallback()));
    component.invoke();

    if(!callback.mInvoked) {
        std::cerr << "Signal callback not received correctly." << std::endl;
        return false;
    }

    root.deinitialize();
    return true;
}

QString SignalsTest::getTestName() {
    return "Signals";
}

////////////////////////////////////////////////////////////////

TestComponent::TestComponent(const QString name)
    : dt::Component(name) {}

void TestComponent::onInitialize() {}

void TestComponent::onDeinitialize() {}

void TestComponent::invoke() {
    emit invoked();
}

////////////////////////////////////////////////////////////////

CallbackClass::CallbackClass()
    : mInvoked(false) {}

void CallbackClass::theCallback() {
    std::cout << "Callback was called." << std::endl;
    mInvoked = true;
}

} // namespace SignalsTest
