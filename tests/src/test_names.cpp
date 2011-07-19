
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include "Root.hpp"


#include "scene/Node.hpp"
#include "component/Component.hpp"
#include "component/CameraComponent.hpp"

int main(int argc, char** argv) {
    dt::Root::get_mutable_instance().Initialize(argc, argv);

    dt::Node* node = new dt::Node();
    std::cout << std::endl << node->GetName();

    dt::CameraComponent* comp = new dt::CameraComponent();
    std::cout << std::endl << comp->GetName();

    dt::Root::get_mutable_instance().Deinitialize();
    return 0;
}
