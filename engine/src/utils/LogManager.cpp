
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include "LogManager.hpp"
#include "Root.hpp"

namespace dt {

LogManager::LogManager() {
    //GetLogger("default"); // create default logger
}

void LogManager::Initialize() {}
void LogManager::Deinitialize() {}

LogManager* LogManager::Get() {
    return Root::get_mutable_instance().GetLogManager();
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
