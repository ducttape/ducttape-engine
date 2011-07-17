
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
    const std::string GetType() const  {
        return "testevent";
    }

    Event* Clone() const {
        return new TestEvent(*this);
    }

    void TestEventReceived() {
        mHasBeenReceivedCorrectly = true;
    }

    bool mHasBeenReceivedCorrectly;
};

class TestEventListener : public dt::EventListener {
public:
    void HandleEvent(dt::Event* e) {
        if(e->GetType() == "testevent") {
            ((TestEvent*)e)->TestEventReceived();
        }
        mHasReceivedAnEvent = true;
    }

    bool mHasReceivedAnEvent;
};

int main(int argc, char** argv) {
    dt::Root& root = dt::Root::get_mutable_instance();
    root.Initialize(argc, argv);

    root.GetStringManager()->Add("testevent");

    TestEventListener listener;
    TestEvent event;

    root.GetEventManager()->AddListener(&listener);
    root.GetEventManager()->HandleEvent(&event);

    if(!listener.mHasReceivedAnEvent) {
        std::cerr << "The EventListener has not received any event." << std::endl;
        root.Deinitialize();
        return 0;
    }

    if(!event.mHasBeenReceivedCorrectly) {
        std::cerr << "The Event has not been recognized as a TestEvent." << std::endl;
        root.Deinitialize();
        return 0;
    }

    std::cout << "Events: OK" << std::endl;
    root.Deinitialize();
    return 0;
}
