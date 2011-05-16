#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <string>
#include <iostream>

#include <boost/algorithm/string.hpp>
#include <boost/ptr_container/ptr_vector.hpp>

#include "LogStream.hpp"

namespace dt {

/**
  * A class responsible for logging different messages to their LogStreams. This class can hold different
  * LogStreams and output the messages to these, defined by their log level.
  */
class Logger {
public:
    /**
      * Advanced constructor. Creates a logger with the given name.
      * @param name The name of the logger.
      */
    Logger(const std::string& name = "default");

    // levels are usually (debug|info|warning|error) or self-defined levels

    /**
      * Logs a message with the given level. The level is taked to determine the correct LogStream, which then is used to output the message.
      * Common levels are DEBUG, INFO, WARNING or ERROR. If the LogStream does not exist, one is created with the level name.
      * @param level The level name of the LogStream
      * @param msg The log message
      */
    void Log(const std::string& level, const std::string& msg);

    /**
      * Returns the stream with the given name.
      * @param streamname the name of the stream to find, usually the log level
      * @returns the LogStream
      */
    LogStream* GetStream(const std::string& streamname);

    /**
      * Logs a message at "DEBUG" level.
      * @param msg The log message
      */
    void Debug(const std::string& msg);

    /**
      * Logs a message at "INFO" level.
      * @param msg The log message
      */
    void Info(const std::string& msg);

    /**
      * Logs a message at "WARNING" level.
      * @param msg The log message
      */
    void Warning(const std::string& msg);

    /**
      * Logs a message at "ERROR" level.
      * @param msg The log message
      */
    void Error(const std::string& msg);

    /**
      * Sets the name of the Logger.
      * @param name The new name
      */
    void SetName(const std::string& name);

    /**
      * Returns the name of the Logger.
      * @returns The name of the Logger
      */
    const std::string& GetName() const;

    /**
      * Return the "default" Logger from Root's LogManager.
      * @returns the default Logger
      */
    static Logger& Get();

    /** Return a specific Logger from Root's LogManager.
      * @param name the name of the Logger
      * @returns the Logger if one is found, otherwise it is created
      */
    static Logger& GetByName(const std::string& name);
private:
    boost::ptr_vector<LogStream> mStreams;  //!< The list of Streams owned by this Logger
    std::string mName;                      //!< The name of this Logger
};

}

#endif
