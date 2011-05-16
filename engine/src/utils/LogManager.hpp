#ifndef DUCTTAPE_ENGINE_UTILS_LOGMANAGER
#define DUCTTAPE_ENGINE_UTILS_LOGMANAGER

#include <boost/ptr_container/ptr_map.hpp>

#include "Logger.hpp"

namespace dt {

class LogManager {
public:
    LogManager();

    Logger& GetLogger(); // returns logger "default"
    Logger& GetLogger(const std::string& name);
private:
    boost::ptr_map<std::string, Logger> mLoggers;
};

}

#endif
