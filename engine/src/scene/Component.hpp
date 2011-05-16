#ifndef COMPONENT_HPP
#define COMPONENT_HPP

#include <iostream>

namespace dt {

class Component {
public:
    Component(const std::string& name);

    std::string GetName();

    virtual void Update(float time_delta) = 0;

private:
    std::string mName;
};

}

#endif
