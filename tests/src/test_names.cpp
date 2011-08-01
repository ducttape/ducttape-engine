
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include <Core/Root.hpp>
#include <Graphics/CameraComponent.hpp>
#include <Scene/Component.hpp>
#include <Scene/Node.hpp>

int main(int argc, char** argv) {
    dt::Root::GetInstance().Initialize(argc, argv);

    dt::Node node;
    std::cout << "Node name: " << node.GetName() << std::endl;
    assert(node.GetName() == "Node-1");

    dt::CameraComponent component;
    std::cout << "Component name: " << component.GetName() << std::endl;
    assert(component.GetName() == "Component-2");

    dt::Root::GetInstance().Deinitialize();
    return 0;
}
