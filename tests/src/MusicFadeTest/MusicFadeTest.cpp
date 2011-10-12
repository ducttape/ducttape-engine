
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include "MusicFadeTest/MusicFadeTest.hpp"

#include <Scene/StateManager.hpp>

namespace MusicFadeTest {

bool MusicFadeTest::Run(int argc, char** argv) {
    dt::Game game;
    game.Run(new Main(), argc, argv);
    return true;
}

QString MusicFadeTest::GetTestName() {
    return "MusicFade";
}

////////////////////////////////////////////////////////////////

Main::Main()
    : mRuntime(0) {}

void Main::UpdateStateFrame(double simulation_frame_time) {
    mRuntime += simulation_frame_time;
    if(mRuntime > 2.0) {
        dt::StateManager::Get()->Pop(1);
    }
}

void Main::OnInitialize() {
    dt::Scene* scene = AddScene(new dt::Scene("testscene"));

    QString music1 = "test_music_intro.ogg";
    QString music2 = "test_music_loop.ogg";

    float origin_vol = 20.f;
    dt::MusicComponent* music_component1 = new dt::MusicComponent(music1);
    music_component1->SetVolume(origin_vol);
    dt::MusicComponent* music_component2 = new dt::MusicComponent(music2);
    music_component2->SetVolume(0.0f);

    auto node = scene->AddChildNode(new dt::Node("music_node"));
    node->AddComponent(music_component1);
    node->AddComponent(music_component2);

    music_component1->Fade(2.0, 0.0f);

    music_component2->Fade(2.0, origin_vol);
}

}
