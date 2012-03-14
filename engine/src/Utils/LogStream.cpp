
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include <Utils/LogStream.hpp>

#include <Utils/Logger.hpp>
#include <Utils/Utils.hpp>

#include <iostream>

namespace dt {

QString LogStream::COLOR_RED = "\033[22;31m";
QString LogStream::COLOR_GREEN = "\033[22;32m";
QString LogStream::COLOR_YELLOW = "\033[01;33m";
QString LogStream::COLOR_BLUE = "\033[01;34m";
QString LogStream::COLOR_PURPLE = "\033[22;35m";
QString LogStream::COLOR_CYAN = "\033[22;36m";
QString LogStream::COLOR_NONE = "\033[0m";

LogStream::LogStream(const QString name)
    : mStream(&std::cout),
      mFormat("[%1 | %2] %3"),        // e.g.: "[default | WARNING] This is a warning!"
      mName(name),
      mDisabled(false) {}

QString LogStream::formatMessage(Logger* logger, const QString& msg) {
    return QString(mFormat).arg(logger->getName()).arg(mName).arg(msg);
}

void LogStream::output(Logger* logger, const QString msg) {
    if(!mDisabled) {
        *mStream << dt::Utils::toStdString(formatMessage(logger, msg)) << std::endl;
    }
}

void LogStream::defaultOutput(Logger* logger, const QString& msg) {
    if(!mDisabled) {
        QString output = QString(mFormat).arg(mName).arg(msg);
        *mStream << dt::Utils::toStdString(output) << std::endl;
    }
}

void LogStream::setStream(std::ostream& stream) {
    mStream = &stream;
}

void LogStream::setName(const QString name) {
    mName = name;
}

void LogStream::setFormat(const QString format) {
    mFormat = format;
}

const QString LogStream::getName() const {
    return mName;
}

void LogStream::setDisabled(bool disabled) {
    mDisabled = disabled;
}

bool LogStream::isDisabled() const {
    return mDisabled;
}

} // namespace dt
