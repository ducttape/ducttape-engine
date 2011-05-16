#include "EnumHelper.hpp"

namespace dt {

EnumHelper::EnumHelper(void* e) {
    mEnumObj = e;
}

sf::Uint32 EnumHelper::Get() {
    return (uint32_t)(*((int*)mEnumObj));
}

void EnumHelper::Set(uint32_t i) {
   *((int*)mEnumObj) = i;
}

sf::Packet& operator << (sf::Packet& p, EnumHelper e) {
    uint32_t i = e.Get();
    p << i;
    return p;
}

sf::Packet& operator >> (sf::Packet& p, EnumHelper e) {
    uint32_t i;
    p >> i;
    e.Set(i);
    return p;
}

}
