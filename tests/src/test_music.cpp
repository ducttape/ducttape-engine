#include "Root.hpp"

#include "component/MusicComponent.hpp"
#include "component/MusicComponentListener.hpp"

#include "event/MusicPauseEvent.hpp"
#include "event/MusicStartEvent.hpp"
#include "event/MusicStopEvent.hpp"

#include "scene/Node.hpp"
#include "scene/Scene.hpp"

int main(int argc, char** argv) {
    dt::Root& root = dt::Root::get_mutable_instance();
    root.Initialize(argc, argv);
    std::string music_file = "test_music_loop.ogg";

    dt::MusicStartEvent start_event;
    dt::MusicPauseEvent pause_event;
    dt::MusicStopEvent stop_event;

    dt::Scene scene("scene1");

    scene.AddChildNode(new dt::Node("music"));

    dt::MusicComponent* music_component = new dt::MusicComponent("music", music_file);

    scene.FindChildNode("music", false)->AddComponent(music_component);

    root.GetEventManager()->AddListener(music_component);

    auto resmgr = root.GetResourceManager();

    if(resmgr->GetMusicFile(music_file)->GetStatus() != sf::Music::Playing) {
        std::cerr << "The music is currently not playing." << std::endl;
        resmgr->GetMusicFile(music_file)->Stop();
        exit(1);
    }

    sf::Sleep(2000);

    if(resmgr->GetMusicFile(music_file)->GetPlayingOffset() < 100) {
        std::cerr << "The music was not played correctly." << std::endl;
        resmgr->GetMusicFile(music_file)->Stop();
        exit(1);
    }

    root.GetEventManager()->HandleEvent(&pause_event);
    if(resmgr->GetMusicFile(music_file)->GetStatus() != sf::Music::Paused) {
        std::cerr << "The music is currently playing. It should be paused." << std::endl;
        resmgr->GetMusicFile(music_file)->Stop();
        exit(1);
    }

    sf::Sleep(200);

    root.GetEventManager()->HandleEvent(&stop_event);
    if(resmgr->GetMusicFile(music_file)->GetStatus() != sf::Music::Stopped) {
        std::cerr << "The music is currently not stopped." << std::endl;
        resmgr->GetMusicFile(music_file)->Stop();
        exit(1);
    }

    sf::Sleep(200);

    root.GetEventManager()->HandleEvent(&start_event);
    if(resmgr->GetMusicFile(music_file)->GetStatus() != sf::Music::Playing) {
        std::cerr << "The music is currently not playing." << std::endl;
        resmgr->GetMusicFile(music_file)->Stop();
        exit(1);
    }

    resmgr->GetMusicFile(music_file)->Stop();
    root.Deinitialize();
    return 0;
}
