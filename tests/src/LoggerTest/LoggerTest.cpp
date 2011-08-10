#include "LoggerTest/LoggerTest.hpp"

namespace LoggerTest {

bool LoggerTest::Run(int argc, char** argv) {
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
    return true;
}

QString LoggerTest::GetTestName() {
    return "Logger";
}

}
