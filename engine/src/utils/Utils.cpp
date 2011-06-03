#include "Utils.hpp"

#include <boost/algorithm/string.hpp>

namespace dt {

std::vector<std::string> split(std::string in, std::string chars) {
    std::vector<std::string> res;
    boost::algorithm::split(res, in, boost::algorithm::is_any_of(chars), boost::algorithm::token_compress_on);
    return res;
}

}
