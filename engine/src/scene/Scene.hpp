#ifndef DUCTTAPE_ENGINE_SCENE_SCENE
#define DUCTTAPE_ENGINE_SCENE_SCENE

#include <string>

#include <boost/ptr_container/ptr_map.hpp>
#include <OGRE/OgreSceneManager.h>

#include "Node.hpp"

namespace dt {

/**
  * A class to represent a whole scene of the game world.
  */
class Scene : public Node {
public:
    /**
      * Default constructor.
      */
    Scene(const std::string& name);

    Ogre::SceneManager* GetSceneManager();

    const std::string& GetName();

protected:
    bool _IsScene();

private:
    std::string mName;
    Node mRootNode; //!< The root Node of the Scene.
};

}

#endif
