
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
    DT_SERIALIZABLE_INHERITED(RaycastComponent, InteractionComponent)
    /**
      * Advanced constructor.
      * @param name The name of the Component.
      * @see Component
      */
    RaycastComponent(const QString& name = "");

    /**
      * Called when Check() is called. It will emit a sCheck signal when it's called and a sHit signal if it hits something.
      * @param start The absolute starting position for the check.
      * @param end The absolute ending position for the check.
      * @see InteractionComponent
      */
    void OnCheck(const btVector3& start, const btVector3& end);
};

}

#endif
