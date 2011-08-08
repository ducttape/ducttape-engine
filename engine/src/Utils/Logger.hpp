
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_UTILS_LOGGER
#define DUCTTAPE_ENGINE_UTILS_LOGGER

#include <Config.hpp>

#include <Utils/LogStream.hpp>

#include <boost/ptr_container/ptr_vector.hpp>

#include <QString>

namespace dt {

/**
  * A class responsible for logging different messages to their LogStreams. This class can hold different
  * LogStreams and output the messages to these, defined by their log level.
  */
class DUCTTAPE_API Logger {
public:
    /**
      * Advanced constructor. Creates a logger with the given name.
      * @param name The name of the logger.
      */
    Logger(const QString& name = "default");

    // levels are usually (debug|info|warning|error) or self-defined levels

    /**
      * Logs a message with the given level. The level is taked to determine the correct LogStream, which then is used to output the message.
      * Common levels are DEBUG, INFO, WARNING or ERROR. If the LogStream does not exist, one is created with the level name.
      * @param level The level name of the LogStream
      * @param msg The log message
      */
    void Log(const QString& level, const QString& msg);

    /**
      * Returns the stream with the given name.
      * @param streamname the name of the stream to find, usually the log level
      * @returns the LogStream
      */
    LogStream* GetStream(const QString& streamname);

    /**
      * Logs a message at "DEBUG" level.
      * @param msg The log message
      */
    void Debug(const QString& msg);

    /**
      * Logs a message at "INFO" level.
      * @param msg The log message
      */
    void Info(const QString& msg);

    /**
      * Logs a message at "WARNING" level.
      * @param msg The log message
      */
    void Warning(const QString& msg);

    /**
      * Logs a message at "ERROR" level.
      * @param msg The log message
      */
    void Error(const QString& msg);

    /**
      * Sets the name of the Logger.
      * @param name The new name
      */
    void SetName(const QString& name);

    /**
      * Returns the name of the Logger.
      * @returns The name of the Logger
      */
    const QString& GetName() const;

    /**
      * Return the "default" Logger from Root's LogManager.
      * @returns the default Logger
      */
    static Logger& Get();

    /** Return a specific Logger from Root's LogManager.
      * @param name the name of the Logger
      * @returns the Logger if one is found, otherwise it is created
      */
    static Logger& GetByName(const QString& name);

private:
    boost::ptr_vector<LogStream> mStreams;  //!< The list of Streams owned by this Logger
    QString mName;                      //!< The name of this Logger
};

} // namespace dt

#endif
