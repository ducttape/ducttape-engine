#ifndef DUCTTAPE_ENGINE_UTILS_ENUMHELPER
#define DUCTTAPE_ENGINE_UTILS_ENUMHELPER

#include <SFML/Network.hpp>

namespace dt {

class EnumHelper {
public:
    EnumHelper(void* e);
    uint32_t Get();
    void Set(uint32_t i);
private:
    void* mEnumObj;
};

sf::Packet& operator << (sf::Packet& p, EnumHelper e);
sf::Packet& operator >> (sf::Packet& p, EnumHelper e);

}

#endif
