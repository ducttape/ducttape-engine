
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include "MusicTest/MusicTest.hpp"

#include <Core/ResourceManager.hpp>
#include <Event/EventManager.hpp>

#include <SFML/System.hpp>

namespace MusicTest {

bool MusicTest::Run(int argc, char** argv) {
    dt::Root& root = dt::Root::GetInstance();
    root.Initialize(argc, argv);
    QString music_file = "test_music_loop.ogg";

    // set global volume
    dt::MusicComponent::SetMasterVolume(2);

    dt::Scene scene("scene1");

    dt::Node* music = scene.AddChildNode(new dt::Node("music"));
    music->AddComponent(new dt::MusicComponent(music_file));

    auto resmgr = root.GetResourceManager();

    if(resmgr->GetMusicFile(music_file)->GetStatus() != sf::Music::Playing) {
        std::cerr << "The music is currently not playing." << std::endl;
        resmgr->GetMusicFile(music_file)->Stop();
        return false;
    }

    sf::Sleep(500);
    dt::MusicComponent::SetMasterVolume(1);
    sf::Sleep(500);

    if(resmgr->GetMusicFile(music_file)->GetPlayingOffset() < 100) {
        std::cerr << "The music was not played correctly." << std::endl;
        resmgr->GetMusicFile(music_file)->Stop();
        return false;
    }

    root.GetEventManager()->InjectEvent(std::make_shared<dt::MusicControlEvent>(dt::MusicControlEvent::PAUSE));
    if(resmgr->GetMusicFile(music_file)->GetStatus() != sf::Music::Paused) {
        std::cerr << "The music is currently playing. It should be paused." << std::endl;
        resmgr->GetMusicFile(music_file)->Stop();
        return false;
    }

    sf::Sleep(200);

    root.GetEventManager()->InjectEvent(std::make_shared<dt::MusicControlEvent>(dt::MusicControlEvent::STOP));
    if(resmgr->GetMusicFile(music_file)->GetStatus() != sf::Music::Stopped) {
        std::cerr << "The music is currently not stopped." << std::endl;
        resmgr->GetMusicFile(music_file)->Stop();
        return false;
    }

    sf::Sleep(200);

    root.GetEventManager()->InjectEvent(std::make_shared<dt::MusicControlEvent>(dt::MusicControlEvent::PLAY));
    if(resmgr->GetMusicFile(music_file)->GetStatus() != sf::Music::Playing) {
        std::cerr << "The music is currently not playing." << std::endl;
        resmgr->GetMusicFile(music_file)->Stop();
        return false;
    }

    resmgr->GetMusicFile(music_file)->Stop();
    root.Deinitialize();
    return true;
}

QString MusicTest::GetTestName() {
    return "Music";
}

}
