
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include "NamesTest/NamesTest.hpp"

#include <Utils/Utils.hpp>

namespace NamesTest {

bool NamesTest::Run(int argc, char** argv) {
    dt::Root::GetInstance().Initialize(argc, argv);

    dt::Node node;
    std::cout << "Node name: " << dt::Utils::ToStdString(node.GetName()) << std::endl;
    if(node.GetName() != "Node-1") {
        std::cerr << "Node name generation failed. Got \"" << dt::Utils::ToStdString(node.GetName()) << "\" instead of \"Node-1\"." << std::endl;
        return false;
    }

    dt::CameraComponent component;
    std::cout << "Component name: " << dt::Utils::ToStdString(component.GetName()) << std::endl;
    assert(component.GetName() == "Component-2");

    if(component.GetName() != "Component-2") {
        std::cerr << "Component name generation failed. Got \"" << dt::Utils::ToStdString(component.GetName()) << "\" instead of \"Component-2\"." << std::endl;
        return false;
    }

    dt::Root::GetInstance().Deinitialize();
    return true;
}

QString NamesTest::GetTestName() {
    return "Names";
}

}
