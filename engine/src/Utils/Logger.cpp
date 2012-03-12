
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

Logger::Logger(const QString name)
    : mName(name) {
    getStream("debug")->setFormat(  dt::LogStream::COLOR_CYAN   + "%2$8s: " + dt::LogStream::COLOR_NONE + "%3$s");
    getStream("info")->setFormat(   dt::LogStream::COLOR_BLUE   + "%2$8s: " + dt::LogStream::COLOR_NONE + "%3$s");
    getStream("error")->setFormat(  dt::LogStream::COLOR_RED    + "%2$8s: " + dt::LogStream::COLOR_NONE + "%3$s");
    getStream("warning")->setFormat(dt::LogStream::COLOR_YELLOW + "%2$8s: " + dt::LogStream::COLOR_NONE + "%3$s");
}

void Logger::log(const QString level, const QString msg) {
    LogStream* stream = getStream(level.toUpper());
    stream->output(this, msg);
}

LogStream* Logger::getStream(const QString streamname) {
    QString name(streamname.toUpper());
    for(auto iter = mStreams.begin(); mStreams.end() != iter; ++iter) {
        if(name == iter->getName().toUpper()) {
            return &(*iter);
        }
    }
    mStreams.push_back(new LogStream(name));
    return &mStreams.back();
}


void Logger::debug(const QString msg) {
    log("DEBUG", msg);
}

void Logger::info(const QString msg) {
    log("INFO", msg);
}

void Logger::warning(const QString msg) {
    log("WARNING", msg);
}

void Logger::error(const QString msg) {
    log("ERROR", msg);
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
