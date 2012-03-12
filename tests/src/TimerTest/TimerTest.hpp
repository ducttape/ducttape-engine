
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_TESTS_TIMERTEST
#define DUCTTAPE_ENGINE_TESTS_TIMERTEST

#include <Config.hpp>

#include "Test.hpp"

#include <Core/Root.hpp>
#include <Scene/Game.hpp>
#include <Scene/State.hpp>
#include <Utils/Timer.hpp>

#include <QObject>

#include <cstdint>

namespace TimerTest {

class TimerTest : public Test {
public:
    bool run(int argc, char** argv);
    QString getTestName();
};

////////////////////////////////////////////////////////////////

class Main : public dt::State {
    Q_OBJECT

public:
    void onInitialize();
    void updateStateFrame(double simulation_frame_time);

private slots:
    void _timerCallback(const QString message);

public:
    std::shared_ptr<dt::Timer> mTimer1;
    std::shared_ptr<dt::Timer> mTimer2;
    //std::shared_ptr<dt::Timer> mTimer3;
    uint16_t mTimer1Count;
    uint16_t mTimer2Count;
    //int mTimer3Count;

    double mTotalTime;
};

} // namespace test_timer

#endif
