
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include "NamesTest/NamesTest.hpp"

#include <Utils/Utils.hpp>

namespace NamesTest {

bool NamesTest::run(int argc, char** argv) {
    dt::Root::getInstance().initialize(argc, argv);

    dt::Node node;
    std::cout << "Node name: " << dt::Utils::toStdString(node.getName()) << std::endl;
    if(node.getName() != "Node-1") {
        std::cerr << "Node name generation failed. Got \"" << dt::Utils::toStdString(node.getName()) << "\" instead of \"Node-1\"." << std::endl;
        return false;
    }

    dt::CameraComponent component;
    std::cout << "Component name: " << dt::Utils::toStdString(component.getName()) << std::endl;
    assert(component.getName() == "Component-2");

    if(component.getName() != "Component-2") {
        std::cerr << "Component name generation failed. Got \"" << dt::Utils::toStdString(component.getName()) << "\" instead of \"Component-2\"." << std::endl;
        return false;
    }

    dt::Root::getInstance().deinitialize();
    return true;
}

QString NamesTest::getTestName() {
    return "Names";
}

}
