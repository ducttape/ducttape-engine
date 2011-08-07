
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_TESTS_TEST_TIMER
#define DUCTTAPE_ENGINE_TESTS_TEST_TIMER

#include <Core/Root.hpp>
#include <Scene/Game.hpp>
#include <Scene/State.hpp>
#include <Utils/Timer.hpp>

#include <QObject>

namespace test_timer {

class Main : public dt::State {
    Q_OBJECT

public:
    void OnInitialize() {
        mTimer1Count = 0;
        mTimer2Count = 0;
        mTimer3Count = 0;

        std::cout << "TIMER: Starting 3 timers:" << std::endl;
        std::cout << "  1 - Event mode  - 100ms" << std::endl;
        std::cout << "  2 - Thread mode - 200ms" << std::endl;
        std::cout << "  3 - Thread mode - 100ms - Callback, no events" << std::endl;

        mTimer1 = std::shared_ptr<dt::Timer>(new dt::Timer("Timer 1 (event mode)", 0.1, true, false));
        mTimer2 = std::shared_ptr<dt::Timer>(new dt::Timer("Timer 2 (thread mode)", 0.2, true, true));
        mTimer3 = std::shared_ptr<dt::Timer>(new dt::Timer("Timer 3 (callback)", 0.1, true, true, false));
        QObject::connect(mTimer3.get(), SIGNAL(TimerTicked(const QString&)),
                         this, SLOT(TimerCallback(const QString&)),
                         Qt::DirectConnection);
        mTotalTime = 0;
    }

    void HandleEvent(std::shared_ptr<dt::Event> e) {
        if(e->GetType() == "DT_TIMERTICKEVENT") {
            std::shared_ptr<dt::TimerTickEvent> t = std::dynamic_pointer_cast<dt::TimerTickEvent>(e);
            bool t1 = (t->GetMessageText() == "Timer 1 (event mode)");
            bool t2 = (t->GetMessageText() == "Timer 2 (thread mode)");

            if(t1) {
                mTimer1Count++;
                std::cout << "Timer tick " << mTimer1Count << ": " << t->GetMessageText().toStdString() << std::endl;
            } else if(t2) {
                mTimer2Count++;
                std::cout << "Timer tick " << mTimer2Count << ": " << t->GetMessageText().toStdString() << std::endl;
            }
        } else if(e->GetType() == "DT_BEGINFRAMEEVENT") {
            mTotalTime += std::dynamic_pointer_cast<dt::BeginFrameEvent>(e)->GetFrameTime();

            if(mTotalTime >= 1.0) {
                dt::StateManager::Get()->Pop(1);
                mTimer1->Stop();
                mTimer2->Stop();
                mTimer3->Stop();
            }
        }
    }

public slots:
    void TimerCallback(const QString& message) {
        mTimer3Count++;
        std::cout << "Timer tick " << mTimer3Count << ": " << message.toStdString() << std::endl;
    }

public:
    std::shared_ptr<dt::Timer> mTimer1;
    std::shared_ptr<dt::Timer> mTimer2;
    std::shared_ptr<dt::Timer> mTimer3;
    int mTimer1Count;
    int mTimer2Count;
    int mTimer3Count;

    double mTotalTime;
};

int Run(int argc, char** argv);

} // namespace test_timer

#endif
