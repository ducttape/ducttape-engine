#include "LogManager.hpp"

namespace dt {

LogManager::LogManager() {
    GetLogger("default"); // create default logger
}

Logger& LogManager::GetLogger() {
    return GetLogger("default");
}

Logger& LogManager::GetLogger(const std::string& name) {
    // create logger with name if not exists
    if(mLoggers.find(name) == mLoggers.end()) {
        mLoggers[name] = Logger(name);
    }
    return mLoggers[name];
}

}
