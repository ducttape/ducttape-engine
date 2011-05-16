#include <iostream>

#include "Root.hpp"

int main() {
    dt::Logger::Get().GetStream("debug")->SetFormat(  dt::LogStream::COLOR_CYAN   + "%2$8s: " + dt::LogStream::COLOR_NONE + "%3$s");
    dt::Logger::Get().GetStream("info")->SetFormat(   dt::LogStream::COLOR_BLUE   + "%2$8s: " + dt::LogStream::COLOR_NONE + "%3$s");
    dt::Logger::Get().GetStream("warning")->SetFormat(dt::LogStream::COLOR_YELLOW + "%2$8s: " + dt::LogStream::COLOR_NONE + "%3$s");
    dt::Logger::Get().GetStream("error")->SetFormat(  dt::LogStream::COLOR_RED    + "%2$8s: " + dt::LogStream::COLOR_NONE + "%3$s");


    dt::Logger::Get().Info("Hello World");

    return 0;
}
