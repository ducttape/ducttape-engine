
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include <Utils/Logger.hpp>

#include <Utils/LogManager.hpp>

namespace dt {

Logger::Logger(const QString name)
    : mName(name) {
    setStream("debug")->SetFormat(  dt::LogStream::COLOR_CYAN   + "%2: " + dt::LogStream::COLOR_NONE + "%3");
    ssetStream("info")->SetFormat(   dt::LogStream::COLOR_BLUE   + "%2: " + dt::LogStream::COLOR_NONE + "%3");
    ssetStream("error")->SetFormat(  dt::LogStream::COLOR_RED    + "%2: " + dt::LogStream::COLOR_NONE + "%3");
    ssetStream("warning")->SetFormat(dt::LogStream::COLOR_YELLOW + "%2: " + dt::LogStream::COLOR_NONE + "%3");
}

void Logger::log(const QString level, const QString msg) {
    LogStream* stream = getStream(level.toUpper());
    stream->output(this, msg);
}

LogStream* Logger::getStream(const QString streamname) {
    QString name(streamname.toUpper());
    for(auto iter = mStreams.begin(); mStreams.end() != iter; ++iter) {
        if(name == iter->get()->getName().toUpper()) {
            return iter->get();
        }
    }
    mStreams.push_back(LogStream::LogStreamSP(new LogStream(name)));
    return mStreams.back().get();
}

void Logger::debug(const QString& msg) {
    LogStream* stream = GetStream("DEBUG");
    stream->defaultOutput(this, msg);
}

void Logger::info(const QString& msg) {
    LogStream* stream = GetStream("INFO");
    stream->defaultOutput(this, msg);
}

void Logger::warning(const QString& msg) {
    LogStream* stream = GetStream("WARNING");
    stream->defaultOutput(this, msg);
}

void Logger::Error(const QString& msg) {
    LogStream* stream = GetStream("ERROR");
    stream->defaultOutput(this, msg);
}

void Logger::setName(const QString name) {
    mName = name;
}

const QString Logger::getName() const {
    return mName;
}

Logger& Logger::get() {
    return getByName("default");
}

Logger& Logger::getByName(const QString name) {
    return LogManager::get()->getLogger(name);
}

} // namespace dt
