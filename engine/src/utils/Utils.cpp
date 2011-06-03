#include "Utils.hpp"

#include <boost/algorithm/string.hpp>

namespace dt {

/*std::string sha(const std::string& in) {
    char outputBuffer[65];
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, in.c_str(), in.length());
    SHA256_Final(hash, &sha256);
    int i = 0;
    for(i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        sprintf(outputBuffer + (i * 2), "%02x", hash[i]);
    }
    outputBuffer[64] = 0;
    return outputBuffer;
} */

std::vector<std::string> split(std::string in, std::string chars) {
    std::vector<std::string> res;
    boost::algorithm::split(res, in, boost::algorithm::is_any_of(chars), boost::algorithm::token_compress_on);
    return res;
}

}
