
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_COMPONENT_RAYCASTCOMPONENT
#define DUCTTAPE_ENGINE_COMPONENT_RAYCASTCOMPONENT

#include <Config.hpp>

#include <Scene/Component.hpp>
#include <Physics/PhysicsBodyComponent.hpp>
#include <Logic/InteractionComponent.hpp>

#include <BulletCollision/CollisionDispatch/btCollisionWorld.h>

#include <QString>

namespace dt {

/**
  * A component using raycast for interacting with other objects in the scene.
  */
class DUCTTAPE_API RaycastComponent : public InteractionComponent {
    Q_OBJECT
public:
    /**
      * Advanced constructor.
      * @param name The name of the Component.
      * @see Component
      */
    RaycastComponent(const QString& name = "");

    /**
      * Called when Check() is called. It will emit a sCheck signal when it's called and a sHit signal if it hits something.
      * @see InteractionComponent
      */
    void OnCheck();

signals:
    void sHit(dt::PhysicsBodyComponent* hit);
    void sCheck(Ogre::Vector3 start, Ogre::Vector3 end);
};

}

#endif