
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include <Utils/LogStream.hpp>

#include <Utils/Logger.hpp>
#include <Utils/Utils.hpp>

#include <boost/format.hpp>

#include <iostream>

namespace dt {

QString LogStream::COLOR_RED = "\033[22;31m";
QString LogStream::COLOR_GREEN = "\033[22;32m";
QString LogStream::COLOR_YELLOW = "\033[01;33m";
QString LogStream::COLOR_BLUE = "\033[01;34m";
QString LogStream::COLOR_PURPLE = "\033[22;35m";
QString LogStream::COLOR_CYAN = "\033[22;36m";
QString LogStream::COLOR_NONE = "\033[0m";

LogStream::LogStream(const QString& name)
    : mStream(&std::cout),
      mFormat("[%1$s | %2$s] %3$s"),        // e.g.: "[default | WARNING] This is a warning!"
      mName(name),
      mDisabled(false) {}

QString LogStream::FormatMessage(Logger* logger, const QString& msg) {
    // return QString(mFormat).arg(logger->GetName()).arg(mName).arg(msg);
    return QString((boost::format(dt::Utils::ToStdString(mFormat)) %
                                  dt::Utils::ToStdString(logger->GetName()) %
                                  dt::Utils::ToStdString(mName) %
                                  dt::Utils::ToStdString(msg)).str().c_str());
}

void LogStream::Output(Logger* logger, const QString& msg) {
    if(!mDisabled) {
        *mStream << dt::Utils::ToStdString(FormatMessage(logger, msg)) << std::endl;
    }
}

void LogStream::SetStream(std::ostream& stream) {
    mStream = &stream;
}

void LogStream::SetName(const QString& name) {
    mName = name;
}

void LogStream::SetFormat(const QString& format) {
    mFormat = format;
}

const QString& LogStream::GetName() const {
    return mName;
}

void LogStream::SetDisabled(bool disabled) {
    mDisabled = disabled;
}

bool LogStream::IsDisabled() const {
    return mDisabled;
}

} // namespace dt
