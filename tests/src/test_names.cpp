
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
    std::cout << "Node name: " << node->GetName() << std::endl;
    assert(node->GetName() == "Node-1");

    dt::CameraComponent* comp = new dt::CameraComponent();
    std::cout << "Component name: " << comp->GetName() << std::endl;
    assert(comp->GetName() == "Component-2");

    dt::Root::get_mutable_instance().Deinitialize();
    return 0;
}
