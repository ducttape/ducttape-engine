
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include "Root.hpp"

int main(int argc, char** argv) {
    dt::Root::get_mutable_instance().Initialize(argc, argv);

    dt::ResourceManager* mgr = dt::ResourceManager::Get();

    if(!mgr->FoundDataPath()) {
        std::cerr << "Data path not found." << std::endl;
        exit(1);
    }

    mgr->AddSoundBuffer("sad-trombone.wav", "trombone");
    if(mgr->GetSoundBuffer("trombone").GetDuration() <= 1000) {
        std::cerr << "The sound buffer is too short, probably empty. Length: " << mgr->GetSoundBuffer("trombone").GetDuration() <<
                     " - Expected: 4150." << std::endl;
        exit(1);
    }

    mgr->AddResourceLocation("sinbad.zip", "Zip");
    // we'd get an Ogre exception if this one failed. No need to check ourselves.

    std::cout << "ResourceManager: OK" << std::endl;
    dt::Root::get_mutable_instance().Deinitialize();
    return 0;
}
