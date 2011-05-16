#include "Logger.hpp"

#include "Root.hpp"

namespace dt {

Logger::Logger(const std::string& name) {
    SetName(name);
    GetStream("debug")->SetFormat(  dt::LogStream::COLOR_CYAN   + "%2$8s: " + dt::LogStream::COLOR_NONE + "%3$s");
    GetStream("info")->SetFormat(   dt::LogStream::COLOR_BLUE   + "%2$8s: " + dt::LogStream::COLOR_NONE + "%3$s");
    GetStream("error")->SetFormat(  dt::LogStream::COLOR_RED    + "%2$8s: " + dt::LogStream::COLOR_NONE + "%3$s");
    GetStream("warning")->SetFormat(dt::LogStream::COLOR_YELLOW + "%2$8s: " + dt::LogStream::COLOR_NONE + "%3$s");
}

void Logger::Log(const std::string& level, const std::string& msg) {
    std::string lvl = boost::to_upper_copy(level);
    LogStream* s = GetStream(lvl);
    s->Output(this, msg);
}

LogStream* Logger::GetStream(const std::string& streamname) {
    std::string name = boost::to_upper_copy(streamname);
    for(auto iter = mStreams.begin(); mStreams.end() != iter; ++iter) {
        std::string sname = boost::to_upper_copy(iter->GetName());
        if(name == sname) {
            return &(*iter);
        }
    }
    mStreams.push_back(new LogStream(name));
    return &mStreams.back();
}


void Logger::Debug(const std::string& msg) {
    Log("DEBUG", msg);
}

void Logger::Info(const std::string& msg) {
    Log("INFO", msg);
}

void Logger::Warning(const std::string& msg) {
    Log("WARNING", msg);
}

void Logger::Error(const std::string& msg) {
    Log("ERROR", msg);
}

void Logger::SetName(const std::string& name) {
    mName = name;
}

const std::string& Logger::GetName() const {
    return mName;
}

Logger& Logger::Get() {
    return GetByName("default");
}

Logger& Logger::GetByName(const std::string& name) {
    return Root::get_mutable_instance().GetLogManager()->GetLogger(name);
}

}
