
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_COMPONENT_GUNCOMPONENT
#define DUCTTAPE_ENGINE_COMPONENT_GUNCOMPONENT

#include <Config.hpp>

#include <Scene/Component.hpp>
#include <Scene/Node.hpp>

#include <BulletCollision/CollisionDispatch/btCollisionWorld.h>

#include <QString>

namespace dt {

/**
  * A component for interacting with other objects in the scene.
  */
class DUCTTAPE_API GunComponent : public Component {
    Q_OBJECT
public:
    /**
      * Advanced constructor.
      * @param name The name of the Component.
      * @see Component
      */
    GunComponent(const QString& name = "");

    void OnCreate();
    void OnDestroy();

    /**
      * Get the nodes hit.
      * @returns The hit nodes.
      */
    Node* Use();

private:
    btCollisionWorld::ClosestRayResultCallback * mRaycastCallback;    //<! The raycast callback.
    Ogre::Vector3 mStart;          //<! The start position of the ray relative to the node.
    Ogre::Vector3 mEnd;            //<! The end position of the ray relative to the node.
    bool mUseRaycast;          //<! Whether raycast is used or not.
};

}

#endif