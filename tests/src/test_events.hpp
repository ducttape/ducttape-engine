
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_TESTS_TEST_EVENTS
#define DUCTTAPE_ENGINE_TESTS_TEST_EVENTS

#include <Core/Root.hpp>
#include <Event/Event.hpp>
#include <Event/EventListener.hpp>
#include <Utils/Logger.hpp>

namespace test_events {

class TestEvent : public dt::Event {
public:
    TestEvent() {
        // This payload only exists to make sure this event can't leak.
        // If this event is created in the wrong way, it will leak the
        // contents of its vector.
        anti_leak_payload.push_back(10);
    }

    const QString GetType() const  {
        return "testevent";
    }

    std::shared_ptr<Event> Clone() const {
        std::shared_ptr<Event> ptr(new TestEvent);
        return ptr;
    }

    std::vector<uint64_t> anti_leak_payload;
};

class CancelEvent : public dt::Event {
public:
    const QString GetType() const  {
        return "cancelevent";
    }

    std::shared_ptr<Event> Clone() const {
        std::shared_ptr<Event> ptr(new CancelEvent);
        return ptr;
    }
};

class CancelListener : public dt::EventListener {
public:
    void HandleEvent(std::shared_ptr<dt::Event> e) {
        if(e->GetType() == "cancelevent") {
            dt::Logger::Get().Info("CancelListener: Canceling event");
            e->Cancel();
        }
    }

    Priority GetEventPriority() const {
        return HIGHEST;
    }
};

class NonCancelListener : public dt::EventListener {
public:
    void HandleEvent(std::shared_ptr<dt::Event> e) {
        if(e->GetType() == "cancelevent") {
            std::cerr << "Error: NonCancelListener should not receive a CancelEvent." << std::endl;
            exit(1);
        }
    }

    Priority GetEventPriority() const {
        return LOWEST;
    }
};



class TestEventListener : public dt::EventListener {
public:
    void HandleEvent(std::shared_ptr<dt::Event> e) {
        if(e->GetType() == "testevent") {
            mHasReceivedTestEvent = true;
        }
        mHasReceivedAnEvent = true;
    }

    bool mHasReceivedAnEvent;
    bool mHasReceivedTestEvent;
};

int Run(int argc, char** argv) {
    dt::Root& root = dt::Root::GetInstance();
    root.Initialize(argc, argv);

    root.GetStringManager()->Add("testevent");

    TestEventListener listener;

    root.GetEventManager()->AddListener(&listener);
    root.GetEventManager()->InjectEvent(std::make_shared<TestEvent>());

    if(!listener.mHasReceivedAnEvent) {
        std::cerr << "The EventListener has not received any event." << std::endl;
        return 1;
    }

    if(!listener.mHasReceivedTestEvent) {
        std::cerr << "The Event has not been recognized as a TestEvent." << std::endl;
        return 1;
    }

    // cancel test
    CancelListener cancel;
    NonCancelListener non_cancel;

    // add the non_cancel listener first to see if they get swapped
    dt::EventManager::Get()->AddListener(&non_cancel);
    dt::EventManager::Get()->AddListener(&cancel);

    // try it out
    dt::EventManager::Get()->InjectEvent(std::make_shared<CancelEvent>());

    std::cout << "Events: OK" << std::endl;
    root.Deinitialize();
    return 0;
}

} // namespace test_events

#endif
