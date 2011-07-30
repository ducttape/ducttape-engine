
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include "LogStream.hpp"

#include "Logger.hpp"
#include <boost/format.hpp>

namespace dt {

std::string LogStream::COLOR_RED = "\033[22;31m";
std::string LogStream::COLOR_GREEN = "\033[22;32m";
std::string LogStream::COLOR_YELLOW = "\033[01;33m";
std::string LogStream::COLOR_BLUE = "\033[01;34m";
std::string LogStream::COLOR_PURPLE = "\033[22;35m";
std::string LogStream::COLOR_CYAN = "\033[22;36m";
std::string LogStream::COLOR_NONE = "\033[0m";

LogStream::LogStream(const std::string& name)
    : mStream(&std::cout),
      mFormat("[%1$s | %2$s] %3$s"),        // e.g.: "[default | WARNING] This is a warning!"
      mName(name),
      mDisabled(false) {}

std::string LogStream::FormatMessage(Logger* logger, const std::string& msg) {
    return (boost::format(mFormat) % logger->GetName() % mName % msg).str();
}

void LogStream::Output(Logger* logger, const std::string& msg) {
    if(!mDisabled) {
        *mStream << FormatMessage(logger, msg) << std::endl;
    }
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

void LogStream::SetDisabled(bool disabled) {
    mDisabled = disabled;
}

bool LogStream::IsDisabled() const {
    return mDisabled;
}

}
