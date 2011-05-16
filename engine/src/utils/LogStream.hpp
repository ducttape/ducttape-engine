#ifndef LOG_LOGSTREAM_HPP
#define LOG_LOGSTREAM_HPP

#include <string>
#include <iostream>
#include <stdio.h>


// forward declaration
class Logger;

class LogStream {
public:
    static std::string COLOR_RED;
    static std::string COLOR_GREEN;
    static std::string COLOR_YELLOW;
    static std::string COLOR_BLUE;
    static std::string COLOR_PURPLE;
    static std::string COLOR_CYAN;
    static std::string COLOR_NONE;

public:
    LogStream(const std::string& name);

    std::string FormatMessage(Logger* logger, const std::string& msg);
    void Output(Logger* logger, const std::string& msg);

    void SetStream(std::ostream& stream);
    void SetName(const std::string& name);
    // %1$s = logger name
    // %2$s = stream name
    // %3$s = message
    void SetFormat(const std::string& format);

    const std::string& GetName() const;
private:
    std::ostream* mStream;
    std::string mFormat; // sprintf
    std::string mName;
};

#endif
