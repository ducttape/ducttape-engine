
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_TESTS_TEST_LOGGER
#define DUCTTAPE_ENGINE_TESTS_TEST_LOGGER

#include <Core/Root.hpp>
#include <Utils/Logger.hpp>

namespace test_logger {

int Run(int argc, char** argv) {
    dt::Root::GetInstance().Initialize(argc, argv);

    std::cout << std::endl << "= default logger test" << std::endl;
    dt::Logger::Get().Debug("DEBUG test");
    dt::Logger::Get().Info("INFO test");
    dt::Logger::Get().Warning("WARNING test");
    dt::Logger::Get().Error("ERROR test");

    std::cout << std::endl << "= custom logger test" << std::endl;
    dt::Logger::GetByName("custom").Debug("DEBUG test");
    dt::Logger::GetByName("custom").Info("INFO test");
    dt::Logger::GetByName("custom").Warning("WARNING test");
    dt::Logger::GetByName("custom").Error("ERROR test");

    std::cout << std::endl << "= custom logger & logstream test" << std::endl;
    dt::Logger::GetByName("custom").Log("TESTSTREAM", "test");

    dt::Root::GetInstance().Deinitialize();
    return 0;
}

}

#endif
