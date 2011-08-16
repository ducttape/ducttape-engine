
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include "EventsTest/EventsTest.hpp"

#include <Core/StringManager.hpp>
#include <Event/EventManager.hpp>

namespace EventsTest {

bool EventsTest::Run(int argc, char** argv) {
    dt::Root& root = dt::Root::GetInstance();
    root.Initialize(argc, argv);

    root.GetStringManager()->Add("testevent");

    TestEventListener listener;

    root.GetEventManager()->AddListener(&listener);
    root.GetEventManager()->InjectEvent(std::make_shared<TestEvent>());

    if(!listener.mHasReceivedAnEvent) {
        std::cerr << "The EventListener has not received any event." << std::endl;
        return false;
    }

    if(!listener.mHasReceivedTestEvent) {
        std::cerr << "The Event has not been recognized as a TestEvent." << std::endl;
        return false;
    }

    // cancel test
    CancelListener cancel;
    NonCancelListener non_cancel;

    // add the non_cancel listener first to see if they get swapped
    dt::EventManager::Get()->AddListener(&non_cancel);
    dt::EventManager::Get()->AddListener(&cancel);

    // try it out
    dt::EventManager::Get()->InjectEvent(std::make_shared<CancelEvent>());

    root.Deinitialize();
    return true;
}

QString EventsTest::GetTestName() {
    return "Events";
}

////////////////////////////////////////////////////////////////

const QString CancelEvent::GetType() const  {
    return "cancelevent";
}

std::shared_ptr<dt::Event> CancelEvent::Clone() const {
    std::shared_ptr<dt::Event> ptr(new CancelEvent);
    return ptr;
}

////////////////////////////////////////////////////////////////

void CancelListener::HandleEvent(std::shared_ptr<dt::Event> e) {
    if(e->GetType() == "cancelevent") {
        dt::Logger::Get().Info("CancelListener: Canceling event");
        e->Cancel();
    }
}

CancelListener::Priority CancelListener::GetEventPriority() const {
    return HIGHEST;
}

////////////////////////////////////////////////////////////////

void NonCancelListener::HandleEvent(std::shared_ptr<dt::Event> e) {
    if(e->GetType() == "cancelevent") {
        std::cerr << "Error: NonCancelListener should not receive a CancelEvent." << std::endl;
        exit(1);
    }
}

NonCancelListener::Priority NonCancelListener::GetEventPriority() const {
    return LOWEST;
}

////////////////////////////////////////////////////////////////

TestEvent::TestEvent() {
    // This payload only exists to make sure this event can't leak.
    // If this event is created in the wrong way, it will leak the
    // contents of its vector.
    anti_leak_payload.push_back(10);
}

const QString TestEvent::GetType() const  {
    return "testevent";
}

std::shared_ptr<dt::Event> TestEvent::Clone() const {
    std::shared_ptr<dt::Event> ptr(new TestEvent);
    return ptr;
}

////////////////////////////////////////////////////////////////

void TestEventListener::HandleEvent(std::shared_ptr<dt::Event> e) {
    if(e->GetType() == "testevent") {
        mHasReceivedTestEvent = true;
    }
    mHasReceivedAnEvent = true;
}

}
