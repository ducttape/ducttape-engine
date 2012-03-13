
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_TESTS_MUSICFADETEST
#define DUCTTAPE_ENGINE_TESTS_MUSICFADETEST

#include <Config.hpp>

#include "Test.hpp"

#include <Audio/MusicComponent.hpp>
#include <Audio/SoundComponent.hpp>
#include <Core/Root.hpp>
#include <Scene/Game.hpp>
#include <Scene/Node.hpp>
#include <Scene/Scene.hpp>

namespace MusicFadeTest {

class MusicFadeTest : public Test {
public:
    bool run(int argc, char** argv);
    QString getTestName();
};

////////////////////////////////////////////////////////////////

class Main : public dt::State {
    Q_OBJECT
public:
    Main();
    void onInitialize();
    void updateStateFrame(double simulation_frame_time);

private:
    double mRuntime;

};

} // namespace MusicFadeTest

#endif
