#ifndef DUCTTAPE_ENGINE_UTILS_STRINGMANAGER
#define DUCTTAPE_ENGINE_UTILS_STRINGMANAGER

#include <string>
#include <map>

namespace dt {

class StringManager {
public:
    StringManager();

    uint32_t Add(const std::string& string);
    bool Has(const std::string& string);
    bool Has(uint32_t id);
    uint32_t Get(const std::string& string);
    const std::string& Get(uint32_t id);
private:
    uint32_t mLastId;
    std::map<uint32_t, std::string> mIds;
};

}

#endif
