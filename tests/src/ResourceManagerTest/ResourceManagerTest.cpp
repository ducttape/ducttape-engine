
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include "ResourceManagerTest/ResourceManagerTest.hpp"

#include <Core/ResourceManager.hpp>

#include <iostream>

namespace ResourceManagerTest {

bool ResourceManagerTest::run(int argc, char** argv) {
    dt::Root::getInstance().initialize(argc, argv);
    dt::ResourceManager* mgr = dt::ResourceManager::get();

    mgr->addSoundBuffer("sad-trombone.wav", "trombone");
    if(mgr->getSoundBuffer("trombone")->getDuration().asMilliseconds() <= 1000) {
        std::cerr << "The sound buffer is too short, probably empty. Length: " << mgr->getSoundBuffer("trombone")->getDuration().asMilliseconds() <<
                     " - Expected: 4150." << std::endl;
        return false;
    }

    mgr->addResourceLocation("sinbad.zip", "Zip");
    // we'd get an Ogre exception if this one failed. No need to check ourselves.

    std::cout << "ResourceManager: OK" << std::endl;
    dt::Root::getInstance().deinitialize();
    return true;
}

QString ResourceManagerTest::getTestName() {
    return "ResourceManager";
}

}
