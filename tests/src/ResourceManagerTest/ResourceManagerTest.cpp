#include "ResourceManagerTest/ResourceManagerTest.hpp"

namespace ResourceManagerTest {

bool ResourceManagerTest::Run(int argc, char** argv) {
    dt::Root::GetInstance().Initialize(argc, argv);
    dt::ResourceManager* mgr = dt::ResourceManager::Get();

    mgr->AddSoundBuffer("sad-trombone.wav", "trombone");
    if(mgr->GetSoundBuffer("trombone")->GetDuration() <= 1000) {
        std::cerr << "The sound buffer is too short, probably empty. Length: " << mgr->GetSoundBuffer("trombone")->GetDuration() <<
                     " - Expected: 4150." << std::endl;
        return false;
    }

    mgr->AddResourceLocation("sinbad.zip", "Zip");
    // we'd get an Ogre exception if this one failed. No need to check ourselves.

    std::cout << "ResourceManager: OK" << std::endl;
    dt::Root::GetInstance().Deinitialize();
    return true;
}

QString ResourceManagerTest::GetTestName() {
    return "ResourceManager";
}

}
