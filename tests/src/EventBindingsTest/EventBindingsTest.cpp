
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include "EventBindingsTest/EventBindingsTest.hpp"

#include <Utils/Utils.hpp>
#include <Core/StringManager.hpp>
#include <Event/EventManager.hpp>

namespace EventBindingsTest {

bool EventBindingsTest::Run(int argc, char** argv) {
    dt::Root& root = dt::Root::GetInstance();
    root.Initialize(argc, argv);

    TestEventListener listener;
    root.GetEventManager()->RegEventType("testTriggerEvent", 65536);
    root.GetEventManager()->RegEventType("testBoundEvent", 65537);
    root.GetEventManager()->AddListener(&listener);

    dt::BindingsManager::Get()->Bind(std::make_shared<dt::SimpleEventBinding>(new TestBoundEvent(42), testTriggerEvent));

    root.GetEventManager()->InjectEvent(std::make_shared<TestTriggerEvent>());

    if(!listener.mHasReceivedTriggerEvent) {
        std::cerr << "The EventListener has not received the trigger event." << std::endl;
        return false;
    }

    if(!listener.mHasReceivedBoundEvent) {
        std::cerr << "The EventListener has not received the bound event." << std::endl;
        return false;
    }

    std::cout << "EventBindings: OK" << std::endl;
    root.Deinitialize();
    return true;
}

QString EventBindingsTest::GetTestName() {
    return "EventBindings";
}

////////////////////////////////////////////////////////////////

uint32_t TestTriggerEvent::GetType() const  {
    return testTriggerEvent;
}

std::shared_ptr<dt::Event> TestTriggerEvent::Clone() const {
    std::shared_ptr<dt::Event> ptr(new TestTriggerEvent());
    return ptr;
}

////////////////////////////////////////////////////////////////

TestBoundEvent::TestBoundEvent(int data)
    : mData(data) {}

uint32_t TestBoundEvent::GetType() const  {
    return testBoundEvent;
}

std::shared_ptr<dt::Event> TestBoundEvent::Clone() const {
    std::shared_ptr<dt::Event> ptr(new TestBoundEvent(mData));
    return ptr;
}

////////////////////////////////////////////////////////////////

void TestEventListener::HandleEvent(std::shared_ptr<dt::Event> e) {
    std::cout << "Received: " << e->GetType() << std::endl;
    if(e->GetType() == testTriggerEvent) {
        mHasReceivedTriggerEvent = true;
    } else if(e->GetType() == testBoundEvent) {
        if(std::dynamic_pointer_cast<TestBoundEvent>(e)->mData == 42) {
            mHasReceivedBoundEvent = true;
        } else {
            std::cerr << "The data has not been transfered correctly." << std::endl;
            exit(1);
        }
    }
}

}
