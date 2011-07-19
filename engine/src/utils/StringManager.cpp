
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include <utility>

#ifdef COMPILER_MSVC
#include <boost/foreach.hpp>
#endif

#include "StringManager.hpp"

namespace dt {

StringManager::StringManager() {
    mLastId = 0;
}

uint32_t StringManager::Add(const std::string& string) {
    if(!Has(string)) {
        mLastId++;
        mIds[mLastId] = string;
        return mLastId;
    } else {
        return Get(string);
    }
}

bool StringManager::Has(const std::string& string) {
#ifdef COMPILER_MSVC
    typedef std::pair<uint32_t, std::string> pair_type;
    BOOST_FOREACH(pair_type i, mIds) {
#else
    for(std::pair<uint32_t, std::string> i: mIds) {
#endif
        if(i.second == string)
            return true;
    }
    return false;
}

bool StringManager::Has(uint32_t id) {
    return mIds.count(id) > 0 && mIds[id] != "";
}

uint32_t StringManager::Get(const std::string& string) {
#ifdef COMPILER_MSVC
    typedef std::pair<uint32_t, std::string> pair_type;
    BOOST_FOREACH(pair_type i, mIds) {
#else
    for(std::pair<uint32_t, std::string> i: mIds) {
#endif
        if(i.second == string)
            return i.first;
    }
    return 0;
}

const std::string& StringManager::Get(uint32_t id) {
    return mIds[id];
}

uint32_t StringManager::GetNextAutoId() {
    mAutoId++;
    return mAutoId;
}

}
