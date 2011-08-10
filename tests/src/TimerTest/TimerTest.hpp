
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_TESTS_TIMERTEST
#define DUCTTAPE_ENGINE_TESTS_TIMERTEST

#include "Test.hpp"

#include <Core/Root.hpp>
#include <Scene/Game.hpp>
#include <Scene/State.hpp>
#include <Utils/Timer.hpp>

#include <QObject>

namespace TimerTest {

class TimerTest : public Test {
public:
    bool Run(int argc, char** argv);
    QString GetTestName();
};

////////////////////////////////////////////////////////////////

class Main : public dt::State {
    Q_OBJECT

public:
    void OnInitialize();
    void HandleEvent(std::shared_ptr<dt::Event> e);

public slots:
    void TimerCallback(const QString& message);

public:
    std::shared_ptr<dt::Timer> mTimer1;
    std::shared_ptr<dt::Timer> mTimer2;
    std::shared_ptr<dt::Timer> mTimer3;
    int mTimer1Count;
    int mTimer2Count;
    int mTimer3Count;

    double mTotalTime;
};

} // namespace test_timer

#endif
