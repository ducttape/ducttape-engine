
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include "SoundTest/SoundTest.hpp"

#include <Utils/Utils.hpp>

#include <SFML/Audio.hpp>
#include <SFML/System.hpp>

namespace SoundTest {

QString SoundTest::getTestName() {
    return "Sound";
}

bool SoundTest::run(int argc, char** argv) {
    dt::Root& root = dt::Root::getInstance();
    root.initialize(argc, argv);

    dt::Scene scene("scene1");
    dt::Node* sound = scene.addChildNode(new dt::Node("sound"));
    dt::SoundComponent* sound_component = new dt::SoundComponent("test_music_loop_mono.ogg");
    sound->addComponent(sound_component);
    sound_component->setMasterVolume(100.0f);
    sound_component->playSound();
    if(sound_component->getSound().getStatus() != sf::Sound::Playing) {
        std::cerr << "The sound is currently not playing." << std::endl;
        exit(1);
    }

    std::cout << "SoundComponent file = " << dt::Utils::toStdString(sound_component->getSoundFileName()) << std::endl;
    std::cout << "SoundComponent duration = " << sound_component->getSound().getBuffer()->getDuration().asMilliseconds() << std::endl;

    /* Test 3D sound */

    for(int i = 15; i > 0; --i) {
        // don't move it "through" the camera
        sound->setPosition(Ogre::Vector3(i, 0, 0));
        sf::sleep(sound_component->getSound().getBuffer()->getDuration()/50.f);
    }

    if(sound_component->getSound().getPlayingOffset().asMilliseconds() < 100) {
        std::cerr << "[1] The sound was not played correctly." << std::endl;
        return false;
    }

    /* Test direct controls */

    sound_component->pauseSound();
    if(sound_component->getSound().getStatus() != sf::Music::Paused) {
        std::cerr << "[2] The music is currently playing. It should be paused." << std::endl;
        return false;
    }

    sf::sleep(sound_component->getSound().getBuffer()->getDuration()/50.f);

    sound_component->stopSound();
    if(sound_component->getSound().getStatus() != sf::Music::Stopped) {
        std::cerr << "[3] The music is currently not stopped." << std::endl;
        return false;
    }

    sf::sleep(sound_component->getSound().getBuffer()->getDuration()/50.f);

    sound_component->playSound();
    if(sound_component->getSound().getStatus() != sf::Music::Playing) {
        std::cerr << "[4] The music is currently not playing." << std::endl;
        return false;
    }

    sf::sleep(sound_component->getSound().getBuffer()->getDuration()/50.f);

    sound->removeComponent(sound_component->getName());
    sound_component = new dt::SoundComponent("sad-trombone.wav");
    sound->addComponent(sound_component);
    sound_component->setMasterVolume(30.0f);
    sound_component->playSound();
    if(sound_component->getSound().getStatus() != sf::Sound::Playing) {
        std::cerr << "The sound is currently not playing." << std::endl;
        return false;
    }

    std::cout << "SoundComponent file = " << dt::Utils::toStdString(sound_component->getSoundFileName()) << std::endl;
    std::cout << "SoundComponent duration = " << sound_component->getSound().getBuffer()->getDuration().asMilliseconds() << std::endl;

    /* Test 3D sound */

    for(int i = 15; i > 0; i--)
    {
        // don't move it "through" the camera
        sound->setPosition(Ogre::Vector3(-i, -10, 0));
            sf::sleep(sound_component->getSound().getBuffer()->getDuration()/20.f);
    }

    if(sound_component->getSound().getPlayingOffset().asMilliseconds() < 100) {
        std::cerr << "[1] The sound was not played correctly." << std::endl;
        return false;
    }

    /* Test direct controls */

    sound_component->pauseSound();
    if(sound_component->getSound().getStatus() != sf::Music::Paused) {
        std::cerr << "[2] The music is currently playing. It should be paused." << std::endl;
        return false;
    }

    sf::sleep(sound_component->getSound().getBuffer()->getDuration()/20.f);

    sound_component->stopSound();
    if(sound_component->getSound().getStatus() != sf::Music::Stopped) {
        std::cerr << "[3] The music is currently not stopped." << std::endl;
        return false;
    }

    sf::sleep(sound_component->getSound().getBuffer()->getDuration()/20.f);

    sound_component->playSound();
    if(sound_component->getSound().getStatus() != sf::Music::Playing) {
        std::cerr << "[4] The music is currently not playing." << std::endl;
        return false;
    }

    sf::sleep(sound_component->getSound().getBuffer()->getDuration()/20.f);
    root.deinitialize();
    return true;
}

}
