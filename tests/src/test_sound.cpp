#include "Root.hpp"

#include "component/SoundComponent.hpp"
#include "component/SoundComponentListener.hpp"

#include "event/SoundPauseEvent.hpp"
#include "event/SoundStartEvent.hpp"
#include "event/SoundStopEvent.hpp"

#include "scene/Node.hpp"
#include "scene/Scene.hpp"

int main(int argc, char** argv) {
    dt::Root& root = dt::Root::get_mutable_instance();
    root.Initialize(argc, argv);
    std::string sound_file = "test_music_loop_mono.ogg";

    dt::SoundStartEvent start_event;
    dt::SoundPauseEvent pause_event;
    dt::SoundStopEvent stop_event;

    dt::Scene scene("scene1");

    scene.AddChildNode(new dt::Node("sound"));

    dt::SoundComponent* sound_component = new dt::SoundComponent("sound", sound_file);

    scene.FindChildNode("sound", false)->AddComponent(sound_component);

    root.GetEventManager()->AddListener(sound_component);

	if(sound_component->GetSound().GetStatus() != sf::Sound::Playing) {
        std::cerr << "The sound is currently not playing." << std::endl;
        exit(1);
    }

	for(int i = 15; i >= -15; i--)
	{
		scene.FindChildNode("sound", false)->SetPosition(Ogre::Vector3(i, 0, 0));
		sf::Sleep(200);
	}

	if(sound_component->GetSound().GetPlayingOffset() < 100) {
        std::cerr << "The sound was not played correctly." << std::endl;
        exit(1);
    }

    root.GetEventManager()->HandleEvent(&pause_event);
    if(sound_component->GetSound().GetStatus() != sf::Music::Paused) {
        std::cerr << "The music is currently playing. It should be paused." << std::endl;
        exit(1);
    }

    sf::Sleep(200);

    root.GetEventManager()->HandleEvent(&stop_event);
    if(sound_component->GetSound().GetStatus() != sf::Music::Stopped) {
        std::cerr << "The music is currently not stopped." << std::endl;
        exit(1);
    }

    sf::Sleep(200);

    root.GetEventManager()->HandleEvent(&start_event);
    if(sound_component->GetSound().GetStatus() != sf::Music::Playing) {
        std::cerr << "The music is currently not playing." << std::endl;
        exit(1);
    }

    root.Deinitialize();
    return 0;
}
