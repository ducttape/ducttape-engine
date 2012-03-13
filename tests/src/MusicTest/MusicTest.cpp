
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include "MusicTest/MusicTest.hpp"

#include <Core/ResourceManager.hpp>

#include <SFML/System.hpp>

namespace MusicTest {

bool MusicTest::run(int argc, char** argv) {
    dt::Root& root = dt::Root::getInstance();
    root.initialize(argc, argv);
    QString music_file = "test_music_loop.ogg";

    // set global volume
    //dt::MusicComponent::SetMasterVolume(2);

    dt::Scene scene("scene1");

    dt::Node* music_node = scene.addChildNode(new dt::Node("music"));
    dt::MusicComponent* music_component = music_node->addComponent(new dt::MusicComponent(music_file));

    music_component->setVolume(40);

    auto resmgr = root.getResourceManager();

    if(resmgr->getMusicFile(music_file)->getStatus() != sf::Music::Playing) {
        std::cerr << "The music is currently not playing." << std::endl;
        resmgr->getMusicFile(music_file)->stop();
        return false;
    }

    sf::sleep(sf::milliseconds(500));
    //dt::MusicComponent::SetMasterVolume(1);
    music_component->setVolume(1);
    sf::sleep(sf::milliseconds(500));

    if(resmgr->getMusicFile(music_file)->getPlayingOffset().asMilliseconds() < 100) {
        std::cerr << "The music was not played correctly." << std::endl;
        resmgr->getMusicFile(music_file)->stop();
        return false;
    }

    music_component->pauseMusic();
    if(resmgr->getMusicFile(music_file)->getStatus() != sf::Music::Paused) {
        std::cerr << "The music is currently playing. It should be paused." << std::endl;
        resmgr->getMusicFile(music_file)->stop();
        return false;
    }

    sf::sleep(sf::milliseconds(200));

    music_component->stopMusic();
    if(resmgr->getMusicFile(music_file)->getStatus() != sf::Music::Stopped) {
        std::cerr << "The music is currently not stopped." << std::endl;
        resmgr->getMusicFile(music_file)->stop();
        return false;
    }

    sf::sleep(sf::milliseconds(200));

    music_component->playMusic();
    if(resmgr->getMusicFile(music_file)->getStatus() != sf::Music::Playing) {
        std::cerr << "The music is currently not playing." << std::endl;
        resmgr->getMusicFile(music_file)->stop();
        return false;
    }

    resmgr->getMusicFile(music_file)->stop();
    root.deinitialize();
    return true;
}

QString MusicTest::getTestName() {
    return "Music";
}

}
