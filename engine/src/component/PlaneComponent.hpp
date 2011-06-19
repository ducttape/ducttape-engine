#ifndef DUCTTAPE_ENGINE_COMPONENT_PLANECOMPONENT
#define DUCTTAPE_ENGINE_COMPONENT_PLANECOMPONENT

#include <string>

#include <OgrePlane.h>
#include <OgreSceneNode.h>
#include <OgreEntity.h>
#include <OgreMeshManager.h>

#include "component/Component.hpp"
#include "component/PlaneComponentListener.hpp"

namespace dt {

class PlaneComponent : public Component {
public:
    /**
      * Advanced constructor.
      * @see Component
      */
    PlaneComponent(const std::string& name, PlaneComponentListener* custom_listener = new PlaneComponentListener());

    virtual void HandleEvent(Event* e);

    void OnActivate();
    void OnDeactivate();
    void OnUpdate(float time_diff);

private:
    Ogre::Plane* mPlane;
    Ogre::SceneNode* mSceneNode;
    Ogre::Entity* mEntity;
};

}

#endif
