
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_TESTS_EVENTBINDINGSTEST
#define DUCTTAPE_ENGINE_TESTS_EVENTBINDINGSTEST

#include "Test.hpp"

#include <Core/Root.hpp>
#include <Utils/Logger.hpp>
#include <Event/Event.hpp>
#include <Event/EventListener.hpp>
#include <Event/SimpleEventBinding.hpp>

#include <memory>

namespace EventBindingsTest {

class EventBindingsTest : public Test {
    bool Run(int argc, char** argv);
    QString GetTestName();
};

////////////////////////////////////////////////////////////////

class TestTriggerEvent : public dt::Event {
public:
    const QString GetType() const;
    std::shared_ptr<dt::Event> Clone() const;
};

////////////////////////////////////////////////////////////////

class TestBoundEvent : public dt::Event {
public:
    TestBoundEvent(int data);
    const QString GetType() const;
    std::shared_ptr<dt::Event> Clone() const;
    int mData;
};

////////////////////////////////////////////////////////////////

class TestEventListener : public dt::EventListener {
public:
    void HandleEvent(std::shared_ptr<dt::Event> e);
    bool mHasReceivedTriggerEvent;
    bool mHasReceivedBoundEvent;
};

} // namespace test_eventbindings

#endif
