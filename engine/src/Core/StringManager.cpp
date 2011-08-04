
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include <Core/StringManager.hpp>

#include <Core/Root.hpp>

namespace dt {

StringManager::StringManager()
    : mLastId(0),
      mAutoId(0) {}

void StringManager::Initialize() {}
void StringManager::Deinitialize() {}

StringManager* StringManager::Get() {
    return Root::GetInstance().GetStringManager();
}

uint32_t StringManager::Add(const QString& string) {
    if(!Has(string)) {
        mLastId++;
        mIds[mLastId] = string;
        return mLastId;
    } else {
        return GetId(string);
    }
}

bool StringManager::Has(const QString& string) {
    for(auto iter = mIds.begin(); iter != mIds.end(); ++iter) {
        if(iter->second == string)
            return true;
    }
    return false;
}

bool StringManager::Has(uint32_t id) {
    return mIds.count(id) > 0 && mIds[id] != "";
}

uint32_t StringManager::GetId(const QString& string) {
    for(auto iter = mIds.begin(); iter != mIds.end(); ++iter) {
        if(iter->second == string)
            return iter->first;
    }
    return 0;
}

const QString& StringManager::GetString(uint32_t id) {
    return mIds[id];
}

uint32_t StringManager::GetNextAutoId() {
    mAutoId++;
    return mAutoId;
}

}
