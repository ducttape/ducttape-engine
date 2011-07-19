
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_COMPONENT_PLANECOMPONENT
#define DUCTTAPE_ENGINE_COMPONENT_PLANECOMPONENT

#include <string>

#include <OgrePlane.h>
#include <OgreSceneNode.h>
#include <OgreEntity.h>
#include <OgreMeshManager.h>

#include "component/Component.hpp"

namespace dt {

class PlaneComponent : public Component {
public:
    /**
      * Advanced constructor.
      * @see Component
      */
    PlaneComponent(const std::string& name = "");

    virtual void HandleEvent(std::shared_ptr<Event> e);

    void OnCreate();
    void OnDestroy();
    void OnUpdate(double time_diff);

private:
    Ogre::Plane* mPlane;
    Ogre::SceneNode* mSceneNode;
    Ogre::Entity* mEntity;
};

}

#endif
