
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include "test_timer.hpp"

#include <Core/Root.hpp>
#include <Scene/Game.hpp>
#include <Scene/State.hpp>
#include <Utils/Timer.hpp>

#include <QObject>

namespace test_timer {

int Run(int argc, char** argv) {
    dt::Game game;
    Main* main = new Main();
    game.Run(main, argc, argv);

    // we expect to have at least 9 ticks of timer 1 and 4 ticks of timer 2 within 10 seconds
    if(main->mTimer1Count < 9) {
        std::cerr << "Timer1 (event) did not tick often enough (only " << main->mTimer1Count << " times)." << std::endl;
        return 1;
    }

    if(main->mTimer2Count < 4) {
        std::cerr << "Timer2 (thread) did not tick often enough (only " << main->mTimer1Count << " times)." << std::endl;
        return 1;
    }

    if(main->mTimer3Count < 9) {
        std::cerr << "Timer3 (thread, callback) did not tick often enough (only " << main->mTimer3Count << " times)." << std::endl;
        return 1;
    }

    std::cout << "TIMERS: OK." << std::endl;

    return 0;
}

} // namespace test_timer
