#ifndef DUCTTAPE_ENGINE_SCENE_COMPONENT
#define DUCTTAPE_ENGINE_SCENE_COMPONENT

#include <string>

namespace dt {

class Component {
public:
    Component();
    Component(const std::string& name);

    const std::string& GetName() const;

    virtual void Update(float time_delta);

private:
    std::string mName;
};

}

#endif
