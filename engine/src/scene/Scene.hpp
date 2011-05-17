#ifndef DUCTTAPE_ENGINE_SCENE_SCENE
#define DUCTTAPE_ENGINE_SCENE_SCENE

#include <string>

#include <boost/ptr_container/ptr_map.hpp>

#include "Node.hpp"

namespace dt {

class Scene {
public:
    /**
      * Default constructor.
      */
    Scene();
private:
    /**
      * @variable The Root Node of the Scene.
      */
    Node mRootNode;
};

}

#endif
