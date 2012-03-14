
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_UTILS_LOGMANAGER
#define DUCTTAPE_ENGINE_UTILS_LOGMANAGER

#include <Config.hpp>

#include <Core/Manager.hpp>
#include <Utils/Logger.hpp>

#include <OgreLogManager.h>

#include <QString>

namespace dt {

/**
  * A manager responsible for holding and providing all Loggers.
  */
class DUCTTAPE_API LogManager : public Manager, public Ogre::LogListener {
    Q_OBJECT
public:
    /**
      * Default constructor.
      */
    LogManager();

    /**
      * Returns the default logger.
      * @returns the default logger (name: "default")
      */
    Logger& getLogger();

    void initialize();
    void deinitialize();

    /**
      * Returns a pointer to the Manager instance.
      * @returns A pointer to the Manager instance.
      */
    static LogManager* get();

    /**
      * Logs an Ogre message to the logger "ogre".
      * @param message The message to be logged.
      * @param level The log level.
      * @param mask_debug If the Ogre log is set up to print to the console.
      * @param log_name The name of the ogre logger.
      * @param skip_message Skip message logs.
      */
#if OGRE_VERSION_MAJOR >= 1 && OGRE_VERSION_MINOR >= 8
    void messageLogged(const Ogre::String& message, Ogre::LogMessageLevel level, bool mask_debug, const Ogre::String& log_name, bool& skip_message);
#else
    void messageLogged(const Ogre::String& message, Ogre::LogMessageLevel level, bool mask_debug, const Ogre::String& log_name);
#endif

    /**
      * Returns the logger with a given name.
      * @param name the name of the Logger to find
      * @returns the Logger if one is found, otherwise creates a new one
      */
    Logger& getLogger(const QString name);

private:
    Ogre::LogManager mOgreLogManager;   //!< The Ogre log manager, which is required to redirect the Ogre log.
    std::map<QString, Logger::LoggerSP> mLoggers;   //!< The list of Loggers, defined by their name
};

} // namespace dt

#endif
