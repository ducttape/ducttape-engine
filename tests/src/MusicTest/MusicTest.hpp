
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_TESTS_MUSICTEST
#define DUCTTAPE_ENGINE_TESTS_MUSICTEST

#include "Test.hpp"

#include <Audio/MusicComponent.hpp>
#include <Audio/MusicControlEvent.hpp>
#include <Audio/SoundComponent.hpp>
#include <Core/Root.hpp>
#include <Scene/Node.hpp>
#include <Scene/Scene.hpp>

namespace MusicTest {

class MusicTest : public Test {
public:
    bool Run(int argc, char** argv);
    QString GetTestName();
};

} // namespace test_music

#endif
