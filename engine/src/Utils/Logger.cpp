
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include <Utils/Logger.hpp>

#include <Utils/LogManager.hpp>

namespace dt {

Logger::Logger(const QString& name)
    : mName(name) {
    GetStream("debug")->SetFormat(  dt::LogStream::COLOR_CYAN   + "%2: " + dt::LogStream::COLOR_NONE + "%3");
    GetStream("info")->SetFormat(   dt::LogStream::COLOR_BLUE   + "%2: " + dt::LogStream::COLOR_NONE + "%3");
    GetStream("error")->SetFormat(  dt::LogStream::COLOR_RED    + "%2: " + dt::LogStream::COLOR_NONE + "%3");
    GetStream("warning")->SetFormat(dt::LogStream::COLOR_YELLOW + "%2: " + dt::LogStream::COLOR_NONE + "%3");
}

void Logger::Log(const QString& level, const QString& msg) {
    LogStream* stream = GetStream(level.toUpper());
    stream->Output(this, msg);
}

LogStream* Logger::GetStream(const QString& streamname) {
    QString name(streamname.toUpper());
    for(auto iter = mStreams.begin(); mStreams.end() != iter; ++iter) {
        if(name == iter->get()->GetName().toUpper()) {
            return iter->get();
        }
    }
    mStreams.push_back(LogStream::LogStreamSP(new LogStream(name)));
    return mStreams.back().get();
}

void Logger::Debug(const QString& msg) {
    LogStream* stream = GetStream("DEBUG");
    stream->DefaultOutput(this, msg);
}

void Logger::Info(const QString& msg) {
    LogStream* stream = GetStream("INFO");
    stream->DefaultOutput(this, msg);
}

void Logger::Warning(const QString& msg) {
    LogStream* stream = GetStream("WARNING");
    stream->DefaultOutput(this, msg);
}

void Logger::Error(const QString& msg) {
    LogStream* stream = GetStream("ERROR");
    stream->DefaultOutput(this, msg);
}

void Logger::SetName(const QString& name) {
    mName = name;
}

const QString& Logger::GetName() const {
    return mName;
}

Logger& Logger::Get() {
    return GetByName("default");
}

Logger& Logger::GetByName(const QString& name) {
    return LogManager::Get()->GetLogger(name);
}

} // namespace dt
