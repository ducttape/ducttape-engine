
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include "Root.hpp"

#include "component/SoundComponent.hpp"

#include "event/SoundsControlEvent.hpp"

#include "scene/Node.hpp"
#include "scene/Scene.hpp"

int main(int argc, char** argv) {
    dt::Root& root = dt::Root::get_mutable_instance();
    root.Initialize(argc, argv);

    dt::Scene scene("scene1");
    scene.AddChildNode(new dt::Node("sound"));
    dt::SoundComponent* sound_component = new dt::SoundComponent("sound", "test_music_loop_mono.ogg");
    scene.FindChildNode("sound", false)->AddComponent(sound_component);

    sound_component = scene.FindChildNode("sound", false)->FindComponent<dt::SoundComponent>("sound");
    sound_component->PlaySound();

	if(sound_component->GetSound().GetStatus() != sf::Sound::Playing) {
        std::cerr << "The sound is currently not playing." << std::endl;
        exit(1);
    }

    /* Test 3D sound */

	for(int i = 15; i >= -15; i--)
	{
        // don't move it "through" the camera
        scene.FindChildNode("sound", false)->SetPosition(Ogre::Vector3(i, 10, 0));
		sf::Sleep(200);
	}

	if(sound_component->GetSound().GetPlayingOffset() < 100) {
        std::cerr << "[1] The sound was not played correctly." << std::endl;
        exit(1);
    }

    /* Test direct controls */

    sound_component->PauseSound();
    if(sound_component->GetSound().GetStatus() != sf::Music::Paused) {
        std::cerr << "[2] The music is currently playing. It should be paused." << std::endl;
        exit(1);
    }

    sf::Sleep(200);

    // root.GetEventManager()->InjectEvent(&stop_event);
    sound_component->StopSound();
    if(sound_component->GetSound().GetStatus() != sf::Music::Stopped) {
        std::cerr << "[3] The music is currently not stopped." << std::endl;
        exit(1);
    }

    sf::Sleep(200);

    sound_component->PlaySound();
    if(sound_component->GetSound().GetStatus() != sf::Music::Playing) {
        std::cerr << "[4] The music is currently not playing." << std::endl;
        exit(1);
    }

    /* Test the SoundsControlEvent */

    root.GetEventManager()->InjectEvent(new dt::SoundsControlEvent(dt::SoundsControlEvent::PAUSE));
    if(sound_component->GetSound().GetStatus() != sf::Music::Paused) {
        std::cerr << "[5] The music is currently playing. It should be paused." << std::endl;
        exit(1);
    }

    sf::Sleep(200);

    root.GetEventManager()->InjectEvent(new dt::SoundsControlEvent(dt::SoundsControlEvent::STOP));
    if(sound_component->GetSound().GetStatus() != sf::Music::Stopped) {
        std::cerr << "[6] The music is currently not stopped." << std::endl;
        exit(1);
    }

    sf::Sleep(200);

    root.GetEventManager()->InjectEvent(new dt::SoundsControlEvent(dt::SoundsControlEvent::PLAY));
    if(sound_component->GetSound().GetStatus() != sf::Music::Playing) {
        std::cerr << "[7] The music is currently not playing." << std::endl;
        exit(1);
    }

    sf::Sleep(200);
    root.Deinitialize();
    return 0;
}
