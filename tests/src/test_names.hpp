
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_TESTS_TEST_NAMES
#define DUCTTAPE_ENGINE_TESTS_TEST_NAMES

#include <Core/Root.hpp>
#include <Graphics/CameraComponent.hpp>
#include <Scene/Component.hpp>
#include <Scene/Node.hpp>

namespace test_names {

int Run(int argc, char** argv) {
    dt::Root::GetInstance().Initialize(argc, argv);

    dt::Node node;
    std::cout << "Node name: " << node.GetName().toStdString() << std::endl;
    if(node.GetName() != "Node-1") {
        std::cerr << "Node name generation failed. Got \"" << node.GetName().toStdString() << "\" instead of \"Node-1\"." << std::endl;
        exit(1);
    }

    dt::CameraComponent component;
    std::cout << "Component name: " << component.GetName().toStdString() << std::endl;
    assert(component.GetName() == "Component-2");

    if(component.GetName() != "Component-2") {
        std::cerr << "Component name generation failed. Got \"" << component.GetName().toStdString() << "\" instead of \"Component-2\"." << std::endl;
        exit(1);
    }

    dt::Root::GetInstance().Deinitialize();
    return 0;
}

} // namespace test_names

#endif
