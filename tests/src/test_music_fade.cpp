
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include "Root.hpp"
#include "game/Game.hpp"

#include "component/SoundComponent.hpp"
#include "component/MusicComponent.hpp"

#include "event/MusicPauseEvent.hpp"
#include "event/MusicStartEvent.hpp"
#include "event/MusicStopEvent.hpp"

#include "scene/Node.hpp"
#include "scene/Scene.hpp"

class Game : public dt::Game {
public:
    Game()
        : mScene("gamescene") {
        mRuntime = 0;
    }

    void HandleEvent(std::shared_ptr<dt::Event> e) {
        dt::Game::HandleEvent(e);

        if(e->GetType() == "DT_BEGINFRAMEEVENT") {
            mRuntime += std::dynamic_pointer_cast<dt::BeginFrameEvent>(e)->GetFrameTime();
            if(mRuntime > 5.0) {
                RequestShutdown();
            }
        }
    }

    void OnInitialize() {
        dt::EventManager::Get()->AddListener(&mScene);
        std::string music1 = "test_music_intro.ogg";
        std::string music2 = "test_music_loop.ogg";

        mScene.AddChildNode(new dt::Node("music_node"));

        dt::MusicComponent* music_component1 = new dt::MusicComponent("music1", music1);

        dt::MusicComponent* music_component2 = new dt::MusicComponent("music2", music2);

        mScene.FindChildNode("music_node", false)->AddComponent(music_component1);

        mScene.FindChildNode("music_node", false)->AddComponent(music_component2);

        auto resmgr = dt::ResourceManager::Get();

        float originVol = 100.0f;

        resmgr->GetMusicFile(music1)->SetVolume(originVol);

        music_component1->Fade(5.0, 0.0f);

        resmgr->GetMusicFile(music2)->SetVolume(0.0f);

        music_component2->Fade(5.0, originVol);
    }

private:
    double mRuntime;
    dt::Scene mScene;

};

int main(int argc, char* argv[]) {

    Game g;
    g.Run(argc, argv);
    return 0;
}