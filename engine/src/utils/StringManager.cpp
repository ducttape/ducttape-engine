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
        return 0;
    }
}

bool StringManager::Has(const std::string& string) {
    for(std::pair<uint32_t, std::string> i: mIds) {
        if(i.second == string)
            return true;
    }
    return false;
}

bool StringManager::Has(uint32_t id) {
    return mIds.count(id) > 0;
}

uint32_t StringManager::Get(const std::string& string) {
    for(std::pair<uint32_t, std::string> i: mIds) {
        if(i.second == string)
            return i.first;
    }
    return 0;
}

const std::string& StringManager::Get(uint32_t id) {
    if(Has(id))
        return mIds[id];
    else
        return "";
}


}
