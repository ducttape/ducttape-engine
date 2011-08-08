
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_TESTS_TEST_RESOURCEMANAGER
#define DUCTTAPE_ENGINE_TESTS_TEST_RESOURCEMANAGER

#include <Core/Root.hpp>

namespace test_resourcemanager {

int Run(int argc, char** argv) {
    dt::Root::GetInstance().Initialize(argc, argv);

    dt::ResourceManager* mgr = dt::ResourceManager::Get();

    mgr->AddSoundBuffer("sad-trombone.wav", "trombone");
    if(mgr->GetSoundBuffer("trombone")->GetDuration() <= 1000) {
        std::cerr << "The sound buffer is too short, probably empty. Length: " << mgr->GetSoundBuffer("trombone")->GetDuration() <<
                     " - Expected: 4150." << std::endl;
        exit(1);
    }

    mgr->AddResourceLocation("sinbad.zip", "Zip");
    // we'd get an Ogre exception if this one failed. No need to check ourselves.

    std::cout << "ResourceManager: OK" << std::endl;
    dt::Root::GetInstance().Deinitialize();
    return 0;
}

} // namespace test_resourcemanager

#endif
