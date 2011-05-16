#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <string>
#include <iostream>

#include <boost/ptr_container/ptr_map.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/foreach.hpp>

#include "LogStream.hpp"

class Logger {
public:
    Logger(); // for boost::ptr_map
    Logger(const std::string& name);

    // levels are usually (debug|info|warning|error) or self-defined levels
    void Log(const std::string& level, const std::string& msg);
    LogStream* GetStream(const std::string& streamname);

    void Debug(const std::string& msg);
    void Info(const std::string& msg);
    void Warning(const std::string& msg);
    void Error(const std::string& msg);

    void SetName(const std::string& name);
    const std::string& GetName() const;
private:
    boost::ptr_vector<LogStream> mStreams;
    std::string mName;
};

#endif
