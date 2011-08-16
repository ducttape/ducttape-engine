
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

    root.GetStringManager()->Add("testtriggerevent");
    root.GetStringManager()->Add("testboundevent");

    TestEventListener listener;
    root.GetEventManager()->AddListener(&listener);

    dt::BindingsManager::Get()->Bind(std::make_shared<dt::SimpleEventBinding>(new TestBoundEvent(42), "testtriggerevent"));

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

const QString TestTriggerEvent::GetType() const  {
    return "testtriggerevent";
}

std::shared_ptr<dt::Event> TestTriggerEvent::Clone() const {
    std::shared_ptr<dt::Event> ptr(new TestTriggerEvent());
    return ptr;
}

////////////////////////////////////////////////////////////////

TestBoundEvent::TestBoundEvent(int data)
    : mData(data) {}

const QString TestBoundEvent::GetType() const  {
    return "testboundevent";
}

std::shared_ptr<dt::Event> TestBoundEvent::Clone() const {
    std::shared_ptr<dt::Event> ptr(new TestBoundEvent(mData));
    return ptr;
}

////////////////////////////////////////////////////////////////

void TestEventListener::HandleEvent(std::shared_ptr<dt::Event> e) {
    std::cout << "Received: " << dt::Utils::ToStdString(e->GetType()) << std::endl;
    if(e->GetType() == "testtriggerevent") {
        mHasReceivedTriggerEvent = true;
    } else if(e->GetType() == "testboundevent") {
        if(std::dynamic_pointer_cast<TestBoundEvent>(e)->mData == 42) {
            mHasReceivedBoundEvent = true;
        } else {
            std::cerr << "The data has not been transfered correctly." << std::endl;
            exit(1);
        }
    }
}

}
