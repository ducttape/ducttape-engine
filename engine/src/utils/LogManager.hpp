
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_UTILS_LOGMANAGER
#define DUCTTAPE_ENGINE_UTILS_LOGMANAGER

#include <boost/ptr_container/ptr_map.hpp>

#include "Logger.hpp"

namespace dt {

/**
  * A manager responsible for holding and providing all Loggers.
  */
class LogManager {
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

    /**
      * Returns the logger with a given name.
      * @param name the name of the Logger to find
      * @returns the Logger if one is found, otherwise creates a new one
      */
    Logger& GetLogger(const std::string& name);
private:
    boost::ptr_map<std::string, Logger> mLoggers;   //!< The list of Loggers, defined by their name
};

}

#endif
