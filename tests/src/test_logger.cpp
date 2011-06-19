#include "Root.hpp"

#include "utils/Logger.hpp"

int main(int argc, char** argv) {
    dt::Root::get_mutable_instance().Initialize(argc, argv);

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

    dt::Root::get_mutable_instance().Deinitialize();
    return 0;
}
