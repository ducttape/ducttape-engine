
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include "Utils.hpp"

#include <boost/algorithm/string.hpp>

namespace dt {

std::vector<std::string> split(std::string in, std::string chars) {
    std::vector<std::string> res;
    boost::algorithm::split(res, in, boost::algorithm::is_any_of(chars), boost::algorithm::token_compress_on);
    return res;
}

}
