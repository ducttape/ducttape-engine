
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
    dt::Node* sound = scene.AddChildNode(new dt::Node("sound"));
    dt::SoundComponent* sound_component =
        new dt::SoundComponent("test_music_loop_mono.ogg");
    sound->AddComponent(sound_component);
    sound_component->PlaySound();
	if(sound_component->GetSound().GetStatus() != sf::Sound::Playing) {
        std::cerr << "The test music loop is currently not playing." << std::endl;
        exit(1);
    }

    std::cout << "SoundComponent file = " << sound_component->GetSoundFile() << std::endl;
    std::cout << "Duration of SoundComponent = " << sound_component->GetSound().GetBuffer()->GetDuration() << std::endl;

    /* Test 3D sound */

	for(int i = 15; i > 0; i--)
	{
        // don't move it "through" the camera
        sound->SetPosition(Ogre::Vector3(i, 10, 0));
		sf::Sleep((sound_component->GetSound().GetBuffer()->GetDuration())/20.f);
	}

	if(sound_component->GetSound().GetPlayingOffset() < 100) {
        std::cerr << "[1] The test music loop was not played correctly." << std::endl;
        exit(1);
    }

    /* Test direct controls */

    sound_component->PauseSound();
    if(sound_component->GetSound().GetStatus() != sf::Music::Paused) {
        std::cerr << "[2] The test music loop is currently playing. It should be paused." << std::endl;
        exit(1);
    }

    sf::Sleep((sound_component->GetSound().GetBuffer()->GetDuration())/20.f);

    // root.GetEventManager()->InjectEvent(&stop_event);
    sound_component->StopSound();
    if(sound_component->GetSound().GetStatus() != sf::Music::Stopped) {
        std::cerr << "[3] The test music loop is currently not stopped." << std::endl;
        exit(1);
    }

    sf::Sleep((sound_component->GetSound().GetBuffer()->GetDuration())/20.f);

    sound_component->PlaySound();
    if(sound_component->GetSound().GetStatus() != sf::Music::Playing) {
        std::cerr << "[4] The test music loop is currently not playing." << std::endl;
        exit(1);
    }

    /* Test the SoundsControlEvent */

    root.GetEventManager()->InjectEvent(std::make_shared<dt::SoundsControlEvent>(dt::SoundsControlEvent::PAUSE));
    if(sound_component->GetSound().GetStatus() != sf::Music::Paused) {
        std::cerr << "[5] The test music loop is currently playing. It should be paused." << std::endl;
        exit(1);
    }

    sf::Sleep((sound_component->GetSound().GetBuffer()->GetDuration())/20.f);

    root.GetEventManager()->InjectEvent(std::make_shared<dt::SoundsControlEvent>(dt::SoundsControlEvent::STOP));
    if(sound_component->GetSound().GetStatus() != sf::Music::Stopped) {
        std::cerr << "[6] The test music loop is currently not stopped." << std::endl;
        exit(1);
    }

    sf::Sleep((sound_component->GetSound().GetBuffer()->GetDuration())/20.f);

    root.GetEventManager()->InjectEvent(std::make_shared<dt::SoundsControlEvent>(dt::SoundsControlEvent::PLAY));
    if(sound_component->GetSound().GetStatus() != sf::Music::Playing) {
        std::cerr << "[7] The test music loop is currently not playing." << std::endl;
        exit(1);
    }

    sf::Sleep((sound_component->GetSound().GetBuffer()->GetDuration())/20.f);

    sound->RemoveComponent(sound_component->GetName());
    sound_component =
        new dt::SoundComponent("sad-trombone.wav");
    sound->AddComponent(sound_component);
    sound_component->PlaySound();
	if(sound_component->GetSound().GetStatus() != sf::Sound::Playing) {
        std::cerr << "The sad trombone is currently not playing." << std::endl;
        exit(1);
    }

    std::cout << "SoundComponent file = " << sound_component->GetSoundFile() << std::endl;
    std::cout << "Duration of SoundComponent = " << sound_component->GetSound().GetBuffer()->GetDuration() << std::endl;

    /* Test 3D sound */

	for(int i = 15; i > 0; i--)
	{
        // don't move it "through" the camera
        sound->SetPosition(Ogre::Vector3(-i, -10, 0));
		sf::Sleep((sound_component->GetSound().GetBuffer()->GetDuration())/20.f);
	}

	if(sound_component->GetSound().GetPlayingOffset() < 100) {
        std::cerr << "[1] The sad trombone was not played correctly." << std::endl;
        exit(1);
    }

    /* Test direct controls */

    sound_component->PauseSound();
    if(sound_component->GetSound().GetStatus() != sf::Music::Paused) {
        std::cerr << "[2] The sad trombone is currently playing. It should be paused." << std::endl;
        exit(1);
    }

    sf::Sleep((sound_component->GetSound().GetBuffer()->GetDuration())/20.f);

    // root.GetEventManager()->InjectEvent(&stop_event);
    sound_component->StopSound();
    if(sound_component->GetSound().GetStatus() != sf::Music::Stopped) {
        std::cerr << "[3] The sad trombone is currently not stopped." << std::endl;
        exit(1);
    }

    sf::Sleep((sound_component->GetSound().GetBuffer()->GetDuration())/20.f);

    sound_component->PlaySound();
    if(sound_component->GetSound().GetStatus() != sf::Music::Playing) {
        std::cerr << "[4] The sad trombone is currently not playing." << std::endl;
        exit(1);
    }

    /* Test the SoundsControlEvent */

    root.GetEventManager()->InjectEvent(std::make_shared<dt::SoundsControlEvent>(dt::SoundsControlEvent::PAUSE));
    if(sound_component->GetSound().GetStatus() != sf::Music::Paused) {
        std::cerr << "[5] The sad trombone is currently playing. It should be paused." << std::endl;
        exit(1);
    }

    sf::Sleep((sound_component->GetSound().GetBuffer()->GetDuration())/20.f);

    root.GetEventManager()->InjectEvent(std::make_shared<dt::SoundsControlEvent>(dt::SoundsControlEvent::STOP));
    if(sound_component->GetSound().GetStatus() != sf::Music::Stopped) {
        std::cerr << "[6] The sad trombone is currently not stopped." << std::endl;
        exit(1);
    }

    sf::Sleep((sound_component->GetSound().GetBuffer()->GetDuration())/20.f);

    root.GetEventManager()->InjectEvent(std::make_shared<dt::SoundsControlEvent>(dt::SoundsControlEvent::PLAY));
    if(sound_component->GetSound().GetStatus() != sf::Music::Playing) {
        std::cerr << "[7] The sad trombone is currently not playing." << std::endl;
        exit(1);
    }

    sf::Sleep((sound_component->GetSound().GetBuffer()->GetDuration())/20.f);
   root.Deinitialize();
    return 0;
}
