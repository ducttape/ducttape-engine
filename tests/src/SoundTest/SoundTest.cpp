
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include "SoundTest/SoundTest.hpp"

#include <Utils/Utils.hpp>
#include <Event/EventManager.hpp>

#include <SFML/Audio.hpp>
#include <SFML/System.hpp>

namespace SoundTest {

QString SoundTest::GetTestName() {
    return "Sound";
}

bool SoundTest::Run(int argc, char** argv) {
    dt::Root& root = dt::Root::GetInstance();
    root.Initialize(argc, argv);

    dt::Scene scene("scene1");
    dt::Node* sound = scene.AddChildNode(new dt::Node("sound"));
    dt::SoundComponent* sound_component =
        new dt::SoundComponent("test_music_loop_mono.ogg");
    sound->AddComponent(sound_component);
    sound_component->PlaySound();
    if(sound_component->GetSound().GetStatus() != sf::Sound::Playing) {
        std::cerr << "The sound is currently not playing." << std::endl;
        exit(1);
    }

    std::cout << "SoundComponent file = " << dt::Utils::ToStdString(sound_component->GetSoundFile()) << std::endl;
    std::cout << "SoundComponent duration = " << sound_component->GetSound().GetBuffer()->GetDuration() << std::endl;

    /* Test 3D sound */

    for(int i = 15; i > 0; --i) {
        // don't move it "through" the camera
        sound->SetPosition(Ogre::Vector3(i, 10, 0));
        sf::Sleep(sound_component->GetSound().GetBuffer()->GetDuration()/50.f);
    }

    if(sound_component->GetSound().GetPlayingOffset() < 100) {
        std::cerr << "[1] The sound was not played correctly." << std::endl;
        return false;
    }

    /* Test direct controls */

    sound_component->PauseSound();
    if(sound_component->GetSound().GetStatus() != sf::Music::Paused) {
        std::cerr << "[2] The music is currently playing. It should be paused." << std::endl;
        return false;
    }

    sf::Sleep(sound_component->GetSound().GetBuffer()->GetDuration()/50.f);

    // root.GetEventManager()->InjectEvent(&stop_event);
    sound_component->StopSound();
    if(sound_component->GetSound().GetStatus() != sf::Music::Stopped) {
        std::cerr << "[3] The music is currently not stopped." << std::endl;
        return false;
    }

    sf::Sleep(sound_component->GetSound().GetBuffer()->GetDuration()/50.f);

    sound_component->PlaySound();
    if(sound_component->GetSound().GetStatus() != sf::Music::Playing) {
        std::cerr << "[4] The music is currently not playing." << std::endl;
        return false;
    }

    /* Test the SoundsControlEvent */

    root.GetEventManager()->InjectEvent(std::make_shared<dt::SoundsControlEvent>(dt::SoundsControlEvent::PAUSE));
    if(sound_component->GetSound().GetStatus() != sf::Music::Paused) {
        std::cerr << "[5] The music is currently playing. It should be paused." << std::endl;
        return false;
    }

    sf::Sleep(sound_component->GetSound().GetBuffer()->GetDuration()/50.f);

    root.GetEventManager()->InjectEvent(std::make_shared<dt::SoundsControlEvent>(dt::SoundsControlEvent::STOP));
    if(sound_component->GetSound().GetStatus() != sf::Music::Stopped) {
        std::cerr << "[6] The music is currently not stopped." << std::endl;
        return false;
    }

    sf::Sleep(sound_component->GetSound().GetBuffer()->GetDuration()/50.f);

    root.GetEventManager()->InjectEvent(std::make_shared<dt::SoundsControlEvent>(dt::SoundsControlEvent::PLAY));
    if(sound_component->GetSound().GetStatus() != sf::Music::Playing) {
        std::cerr << "[7] The music is currently not playing." << std::endl;
        return false;
    }

    sf::Sleep(sound_component->GetSound().GetBuffer()->GetDuration()/50.f);

    sound->RemoveComponent(sound_component->GetName());
    sound_component = new dt::SoundComponent("sad-trombone.wav");
    sound_component->SetVolume(5.f);
    sound->AddComponent(sound_component);
    sound_component->PlaySound();
    if(sound_component->GetSound().GetStatus() != sf::Sound::Playing) {
        std::cerr << "The sound is currently not playing." << std::endl;
        return false;
    }

    std::cout << "SoundComponent file = " << dt::Utils::ToStdString(sound_component->GetSoundFile()) << std::endl;
    std::cout << "SoundComponent duration = " << sound_component->GetSound().GetBuffer()->GetDuration() << std::endl;

    /* Test 3D sound */

    for(int i = 15; i > 0; i--)
    {
        // don't move it "through" the camera
        sound->SetPosition(Ogre::Vector3(-i, -10, 0));
            sf::Sleep(sound_component->GetSound().GetBuffer()->GetDuration()/20.f);
    }

    if(sound_component->GetSound().GetPlayingOffset() < 100) {
        std::cerr << "[1] The sound was not played correctly." << std::endl;
        return false;
    }

    /* Test direct controls */

    sound_component->PauseSound();
    if(sound_component->GetSound().GetStatus() != sf::Music::Paused) {
        std::cerr << "[2] The music is currently playing. It should be paused." << std::endl;
        return false;
    }

    sf::Sleep(sound_component->GetSound().GetBuffer()->GetDuration()/20.f);

    // root.GetEventManager()->InjectEvent(&stop_event);
    sound_component->StopSound();
    if(sound_component->GetSound().GetStatus() != sf::Music::Stopped) {
        std::cerr << "[3] The music is currently not stopped." << std::endl;
        return false;
    }

    sf::Sleep(sound_component->GetSound().GetBuffer()->GetDuration()/20.f);

    sound_component->PlaySound();
    if(sound_component->GetSound().GetStatus() != sf::Music::Playing) {
        std::cerr << "[4] The music is currently not playing." << std::endl;
        return false;
    }

    /* Test the SoundsControlEvent */

    root.GetEventManager()->InjectEvent(std::make_shared<dt::SoundsControlEvent>(dt::SoundsControlEvent::PAUSE));
    if(sound_component->GetSound().GetStatus() != sf::Music::Paused) {
        std::cerr << "[5] The music is currently playing. It should be paused." << std::endl;
        return false;
    }

    sf::Sleep(sound_component->GetSound().GetBuffer()->GetDuration()/20.f);

    root.GetEventManager()->InjectEvent(std::make_shared<dt::SoundsControlEvent>(dt::SoundsControlEvent::STOP));
    if(sound_component->GetSound().GetStatus() != sf::Music::Stopped) {
        std::cerr << "[6] The music is currently not stopped." << std::endl;
        return false;
    }

    sf::Sleep(sound_component->GetSound().GetBuffer()->GetDuration()/20.f);

    root.GetEventManager()->InjectEvent(std::make_shared<dt::SoundsControlEvent>(dt::SoundsControlEvent::PLAY));
    if(sound_component->GetSound().GetStatus() != sf::Music::Playing) {
        std::cerr << "[7] The music is currently not playing." << std::endl;
        return false;
    }

    sf::Sleep(sound_component->GetSound().GetBuffer()->GetDuration()/20.f);
    root.Deinitialize();
    return true;
}

}
