
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

void LogManager::Initialize() {
    // Redirect the Ogre log (create a default log)
    mOgreLogManager.createLog("Ogre.log", true, false)->addListener(this);
    GetLogger("Ogre.log").GetStream("INFO")->SetDisabled(true);
    GetLogger("Ogre.log").GetStream("DEBUG")->SetDisabled(true);
}

void LogManager::Deinitialize() {}

LogManager* LogManager::Get() {
    return Root::get_mutable_instance().GetLogManager();
}

void LogManager::messageLogged(const std::string& message, Ogre::LogMessageLevel level, bool mask_debug, const std::string& log_name) {
    if(level == Ogre::LML_CRITICAL) {
        GetLogger(log_name).Error(message);
    } else if(level == Ogre::LML_NORMAL) {
        GetLogger(log_name).Info(message);
    } else if(level == Ogre::LML_TRIVIAL) {
        GetLogger(log_name).Debug(message);
    }
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
