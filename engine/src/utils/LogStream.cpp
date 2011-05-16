#include "LogStream.hpp"

#include "Logger.hpp"

std::string LogStream::COLOR_RED = "\033[22;31m";
std::string LogStream::COLOR_GREEN = "\033[22;32m";
std::string LogStream::COLOR_YELLOW = "\033[01;33m";
std::string LogStream::COLOR_BLUE = "\033[01;34m";
std::string LogStream::COLOR_PURPLE = "\033[22;35m";
std::string LogStream::COLOR_CYAN = "\033[22;36m";
std::string LogStream::COLOR_NONE = "\033[0m";

LogStream::LogStream(const std::string& name) {
    SetName(name);
    SetStream(std::cout);
    SetFormat("[%1$s | %2$s] %3$s"); // [default | WARNING] This is a warning!
}

std::string LogStream::FormatMessage(Logger* logger, const std::string& msg) {
    char result[256];
    sprintf(result, mFormat.c_str(), logger->GetName().c_str(), mName.c_str(), msg.c_str());
    return result;
}

void LogStream::Output(Logger* logger, const std::string& msg) {
    *mStream << FormatMessage(logger, msg) << std::endl;
}

void LogStream::SetStream(std::ostream& stream) {
    mStream = &stream;
}

void LogStream::SetName(const std::string& name) {
    mName = name;
}

void LogStream::SetFormat(const std::string& format) {
    mFormat = format;
}

const std::string& LogStream::GetName() const {
    return mName;
}
