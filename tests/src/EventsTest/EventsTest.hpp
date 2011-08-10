
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_TESTS_EVENTSTEST
#define DUCTTAPE_ENGINE_TESTS_EVENTSTEST

#include "Test.hpp"

#include <Core/Root.hpp>
#include <Event/Event.hpp>
#include <Event/EventListener.hpp>
#include <Utils/Logger.hpp>

#include <memory>

namespace EventsTest {

class EventsTest : public Test {
public:
    bool Run(int argc, char** argv);
    QString GetTestName();
};

////////////////////////////////////////////////////////////////

class CancelEvent : public dt::Event {
public:
    const QString GetType() const;
    std::shared_ptr<dt::Event> Clone() const;
};

////////////////////////////////////////////////////////////////

class CancelListener : public dt::EventListener {
public:
    void HandleEvent(std::shared_ptr<dt::Event> e);
    Priority GetEventPriority() const;
};

////////////////////////////////////////////////////////////////

class NonCancelListener : public dt::EventListener {
public:
    void HandleEvent(std::shared_ptr<dt::Event> e);
    Priority GetEventPriority() const;
};

////////////////////////////////////////////////////////////////

class TestEvent : public dt::Event {
public:
    TestEvent();
    const QString GetType() const;
    std::shared_ptr<dt::Event> Clone() const;
    std::vector<uint64_t> anti_leak_payload;
};

////////////////////////////////////////////////////////////////

class TestEventListener : public dt::EventListener {
public:
    void HandleEvent(std::shared_ptr<dt::Event> e);
    bool mHasReceivedAnEvent;
    bool mHasReceivedTestEvent;
};

} // namespace EventsTest

#endif
