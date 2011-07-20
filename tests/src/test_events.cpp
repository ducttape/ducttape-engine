
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include "Root.hpp"

#include "utils/Logger.hpp"

#include "event/Event.hpp"
#include "event/EventListener.hpp"

class TestEvent : public dt::Event {
public:
    TestEvent() {
        // This payload only exists to make sure this event can't leak.
        // If this event is created in the wrong way, it will leak the
        // contents of its vector.
        anti_leak_payload.push_back(10);
    }

    const std::string GetType() const  {
        return "testevent";
    }

    std::shared_ptr<Event> Clone() const {
        std::shared_ptr<Event> ptr(new TestEvent);
        return ptr;
    }

    std::vector<uint64_t> anti_leak_payload;
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

int main(int argc, char** argv) {
    dt::Root& root = dt::Root::get_mutable_instance();
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

    std::cout << "Events: OK" << std::endl;
    root.Deinitialize();
    return 0;
}
