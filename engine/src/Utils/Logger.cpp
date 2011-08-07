
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include <Utils/Logger.hpp>

#include <Utils/LogManager.hpp>

#include <boost/algorithm/string.hpp>

namespace dt {

Logger::Logger(const QString& name)
    : mName(name) {
    GetStream("debug")->SetFormat(  dt::LogStream::COLOR_CYAN   + "%2$8s: " + dt::LogStream::COLOR_NONE + "%3$s");
    GetStream("info")->SetFormat(   dt::LogStream::COLOR_BLUE   + "%2$8s: " + dt::LogStream::COLOR_NONE + "%3$s");
    GetStream("error")->SetFormat(  dt::LogStream::COLOR_RED    + "%2$8s: " + dt::LogStream::COLOR_NONE + "%3$s");
    GetStream("warning")->SetFormat(dt::LogStream::COLOR_YELLOW + "%2$8s: " + dt::LogStream::COLOR_NONE + "%3$s");
}

void Logger::Log(const QString& level, const QString& msg) {
    LogStream* stream = GetStream(level.toUpper());
    stream->Output(this, msg);
}

LogStream* Logger::GetStream(const QString& streamname) {
    QString name(streamname.toUpper());
    for(auto iter = mStreams.begin(); mStreams.end() != iter; ++iter) {
        if(name == iter->GetName().toUpper()) {
            return &(*iter);
        }
    }
    mStreams.push_back(new LogStream(name));
    return &mStreams.back();
}


void Logger::Debug(const QString& msg) {
    Log("DEBUG", msg);
}

void Logger::Info(const QString& msg) {
    Log("INFO", msg);
}

void Logger::Warning(const QString& msg) {
    Log("WARNING", msg);
}

void Logger::Error(const QString& msg) {
    Log("ERROR", msg);
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
