
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_UTILS_LOGMANAGER
#define DUCTTAPE_ENGINE_UTILS_LOGMANAGER

#include <boost/ptr_container/ptr_map.hpp>
#include <OGRE/OgreLogManager.h>

#include "Config.hpp"
#include "Logger.hpp"
#include "Manager.hpp"

namespace dt {

/**
  * A manager responsible for holding and providing all Loggers.
  */
class DUCTTAPE_API LogManager : public Manager,
    public Ogre::LogListener {
public:
    /**
      * Default constructor.
      */
    LogManager();

    /**
      * Returns the default logger.
      * @returns the default logger (name: "default")
      */
    Logger& GetLogger();

    void Initialize();
    void Deinitialize();

    /**
      * Returns a pointer to the Manager instance.
      * @returns A pointer to the Manager instance.
      */
    static LogManager* Get();

    /**
      * Logs an Ogre message to the logger "ogre".
      * @param message The message to be logged.
      * @param level The log level.
      * @param mask_debug If the Ogre log is set up to print to the console.
      * @param log_name The name of the ogre logger.
      */
    void messageLogged(const std::string& message, Ogre::LogMessageLevel level, bool mask_debug, const std::string& log_name);

    /**
      * Returns the logger with a given name.
      * @param name the name of the Logger to find
      * @returns the Logger if one is found, otherwise creates a new one
      */
    Logger& GetLogger(const std::string& name);


private:
    Ogre::LogManager mOgreLogManager;   //!< The Ogre log manager, which is required to redirect the Ogre log.
    boost::ptr_map<std::string, Logger> mLoggers;   //!< The list of Loggers, defined by their name
};

}

#endif
