
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_UTILS_LOGSTREAM
#define DUCTTAPE_ENGINE_UTILS_LOGSTREAM

#include <Config.hpp>

#include <ostream>
#include <string>

namespace dt {

class Logger;

/**
  * A class for formatting log messages and writing them to the correct output stream.
  */
class DUCTTAPE_API LogStream {
public:
    static std::string COLOR_RED;       //!< ANSI color code for red
    static std::string COLOR_GREEN;     //!< ANSI color code for green
    static std::string COLOR_YELLOW;    //!< ANSI color code for yellow
    static std::string COLOR_BLUE;      //!< ANSI color code for blue
    static std::string COLOR_PURPLE;    //!< ANSI color code for purple
    static std::string COLOR_CYAN;      //!< ANSI color code for cyan
    static std::string COLOR_NONE;      //!< ANSI color code for no color (reset)

public:
    /**
      * Advanced constructor. Creates a LogStream with the given name.
      * @param name the name of the new LogStream
      */
    LogStream(const std::string& name);

    /**
      * Formats a log message, taking into account the name of the logger.
      * @param logger the Logger the message was sent from
      * @param msg the log message
      * @returns the formatted message
      * @see SetFormat()
      */
    std::string FormatMessage(Logger* logger, const std::string& msg);

    /**
      * Formats and outputs a log message to the output stream.
      * @see FormatMessage()
      * @see SetStream()
      * @param logger the Logger the message was sent from
      * @param msg the log message
      */
    void Output(Logger* logger, const std::string& msg);

    /**
      * Sets the output stream for this LogStream.
      * @param stream the new output stream
      */
    void SetStream(std::ostream& stream);

    /**
      * Sets the name (level) for this LogStream.
      * @param name the new name
      */
    void SetName(const std::string& name);

    /**
      * Sets the format which is used to format the messages before displaying.
      * %1$s = logger name, %2$s = stream name, %3$s = log message
      * @see sprintf
      * @param format the new format string
      */
    void SetFormat(const std::string& format);

    /**
      * Returns the Logger's name.
      * @returns the Logger's name
      */
    const std::string& GetName() const;

    /**
      * Sets whether messages should be written to this stream or not. Useful for disabling some streams, e.g. DEBUG, while keeping
      * more important streams enabled, e.g. ERROR.
      * @param disabled Whether the stream should be disabled or not.
      */
    void SetDisabled(bool disabled);

    /**
      * Returns whether the stream is disabled.
      * @returns Whether the stream is disabled.
      */
    bool IsDisabled() const;
private:
    std::ostream* mStream;  //!< the output stream
    std::string mFormat;    //!< the message format
    std::string mName;      //!< this LogStream's name, also called "log level"
    bool mDisabled;         //!< Whether this LogStream is disabled or not.
};

} // namespace dt

#endif
