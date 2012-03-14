
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include "LoggerTest/LoggerTest.hpp"

#include <iostream>

namespace LoggerTest {

bool LoggerTest::run(int argc, char** argv) {
    dt::Root::getInstance().initialize(argc, argv);

    std::cout << std::endl << "= default logger test" << std::endl;
    dt::Logger::get().debug("DEBUG test");
    dt::Logger::get().info("INFO test");
    dt::Logger::get().warning("WARNING test");
    dt::Logger::get().error("ERROR test");

    std::cout << std::endl << "= custom logger test" << std::endl;
    dt::Logger::getByName("custom").debug("DEBUG test");
    dt::Logger::getByName("custom").info("INFO test");
    dt::Logger::getByName("custom").warning("WARNING test");
    dt::Logger::getByName("custom").error("ERROR test");

    std::cout << std::endl << "= custom logger & logstream test" << std::endl;
    dt::Logger::getByName("custom").log("TESTSTREAM", "test");

    dt::Root::getInstance().deinitialize();
    return true;
}

QString LoggerTest::getTestName() {
    return "Logger";
}

}
