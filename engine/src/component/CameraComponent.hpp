#ifndef DUCTTAPE_ENGINE_COMPONENT_CAMERACOMPONENT 
#define DUCTTAPE_ENGINE_COMPONENT_CAMERACOMPONENT

#include <string>

#include "component/Component.hpp"

namespace dt {

class CameraComponent {
public:
    CameraComponent();

    ~CameraComponent();

    std::string GetName();
};

}

#endif
