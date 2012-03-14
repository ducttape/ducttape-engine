
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

void LogManager::initialize() {
    // Redirect the Ogre log (create a default log)
    mOgreLogManager.createLog("Ogre.log", true, false)->addListener(this);
    getLogger("Ogre.log").getStream("INFO")->setDisabled(true);
    getLogger("Ogre.log").getStream("DEBUG")->setDisabled(true);

    // The MyGUI LogManager configuration currently is done in Graphics/GuiManager.cpp
    // due to initilization order reasons.
}

void LogManager::deinitialize() {}

LogManager* LogManager::get() {
    return Root::getInstance().getLogManager();
}

#if OGRE_VERSION_MAJOR >= 1 && OGRE_VERSION_MINOR >= 8
void LogManager::messageLogged(const Ogre::String& message, Ogre::LogMessageLevel level, bool mask_debug, const Ogre::String& log_name, bool& skip_message) {
    if(level == Ogre::LML_CRITICAL) {
        getLogger(QString(log_name.c_str())).error(QString(message.c_str()));
    } else if(level == Ogre::LML_NORMAL) {
        getLogger(QString(log_name.c_str())).info(QString(message.c_str()));
    } else if(level == Ogre::LML_TRIVIAL) {
        getLogger(QString(log_name.c_str())).debug(QString(message.c_str()));
    }
}
#else
void LogManager::messageLogged(const Ogre::String& message, Ogre::LogMessageLevel level, bool mask_debug, const Ogre::String& log_name) {
    if(level == Ogre::LML_CRITICAL) {
        getLogger(QString(log_name.c_str())).error(QString(message.c_str()));
    } else if(level == Ogre::LML_NORMAL) {
        getLogger(QString(log_name.c_str())).info(QString(message.c_str()));
    } else if(level == Ogre::LML_TRIVIAL) {
        getLogger(QString(log_name.c_str())).debug(QString(message.c_str()));
    }
}
#endif

Logger& LogManager::getLogger() {
    return getLogger("default");
}

Logger& LogManager::getLogger(const QString name) {
    // create logger with name if not exists
    if(mLoggers.find(name) == mLoggers.end()) {
        Logger::LoggerSP shared_ptr(new Logger(name));
        mLoggers[name] = shared_ptr;
    }
    return *(mLoggers[name].get());
}

} // namespace dt
