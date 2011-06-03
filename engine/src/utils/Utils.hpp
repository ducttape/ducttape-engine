#ifndef DUCTTAPE_ENGINE_UTILS_UTILS_HPP
#define DUCTTAPE_ENGINE_UTILS_UTILS_HPP

#include <boost/lexical_cast.hpp>

#include <string>
#include <vector>

namespace dt {

template <typename Source> std::string tostr(const Source& source) {
    return boost::lexical_cast<std::string>(source);
}

template <typename Source> int toint(const Source& source) {
    return boost::lexical_cast<int>(source);
}

std::vector<std::string> split(std::string in, std::string chars);

}

#endif
