
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include <Utils/LogManager.hpp>

#include <Core/Root.hpp>

namespace dt {

LogManager::LogManager() {}

void LogManager::Initialize() {
    // Redirect the Ogre log (create a default log)
    mOgreLogManager.createLog("Ogre.log", true, false)->addListener(this);
    GetLogger("Ogre.log").GetStream("INFO")->SetDisabled(true);
    GetLogger("Ogre.log").GetStream("DEBUG")->SetDisabled(true);

    // The MyGUI LogManager configuration currently is done in Graphics/GuiManager.cpp
    // due to initilization order reasons.
}

void LogManager::Deinitialize() {}

LogManager* LogManager::Get() {
    return Root::GetInstance().GetLogManager();
}

void LogManager::messageLogged(const Ogre::String& message, Ogre::LogMessageLevel level, bool mask_debug, const Ogre::String& log_name) {
    if(level == Ogre::LML_CRITICAL) {
        GetLogger(QString(log_name.c_str())).Error(QString(message.c_str()));
    } else if(level == Ogre::LML_NORMAL) {
        GetLogger(QString(log_name.c_str())).Info(QString(message.c_str()));
    } else if(level == Ogre::LML_TRIVIAL) {
        GetLogger(QString(log_name.c_str())).Debug(QString(message.c_str()));
    }
}

Logger& LogManager::GetLogger() {
    return GetLogger("default");
}

Logger& LogManager::GetLogger(const QString& name) {
    // create logger with name if not exists
    if(mLoggers.find(name) == mLoggers.end()) {
        mLoggers[name] = Logger(name);
    }
    return mLoggers[name];
}

} // namespace dt
