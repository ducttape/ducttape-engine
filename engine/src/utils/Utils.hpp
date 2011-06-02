#ifndef UTIL_UTILS_HPP
#define UTIL_UTILS_HPP

#include <string>
#include <vector>
#include <boost/lexical_cast.hpp>
// #include <openssl/sha.h>

template <typename Source> std::string tostr(const Source& source) {
    return boost::lexical_cast<std::string>(source);
}

template <typename Source> int toint(const Source& source) {
    return boost::lexical_cast<int>(source);
}

// std::string sha(const std::string& in);

std::vector<std::string> split(std::string in, std::string chars);

#endif
