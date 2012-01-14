
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_COMPONENT_COLLISIONCOMPONENT
#define DUCTTAPE_ENGINE_COMPONENT_COLLISIONCOMPONENT

#include <Config.hpp>

#include <Scene/Component.hpp>
#include <Physics/PhysicsBodyComponent.hpp>
#include <Logic/InteractionComponent.hpp>

#include <BulletCollision/CollisionDispatch/btCollisionWorld.h>

#include <QString>

namespace dt {

/**
  * A component using collision detection by shooting bullets for interacting with other objects in the scene.
  */
class DUCTTAPE_API CollisionComponent : public InteractionComponent {
    Q_OBJECT
public:
    /**
      * Advanced constructor.
      * @param bullet_handle The handle o the bullet's mesh.
      * @param name The name of the Component.
      * @see Component
      */
    CollisionComponent(const QString& bullet_handle, const QString& name = "");

    /**
      * Sets the handle of the bullet's mesh.
      * @param bullet_handle The handle to set.
      */
    void SetBulletMeshHandle(const QString& bullet_handle);

    /**
      * Gets the handle of the bullet's mesh.
      * @returns The handle of the bullet's mesh.
      */
    const QString& GetBulletMeshHandle();

    void OnCreate();

protected:
    /*
     * Called when Check() is called. It will emit a sCheck signal.
     * @param start The absolute starting position for the check.
     * @param end The absolute ending position for the check.
     * @see InteractionComponent
     */
    void OnCheck(const btVector3& start, const btVector3& end);

protected slots:
     /**
      * Called when the thrown physics body hits an object. It will emit a sHit signal.
      * @param hit The hit object.
      * @param bullet The bullet object.
      */
    void OnHit(dt::PhysicsBodyComponent* hit, dt::PhysicsBodyComponent* bullet);

private:
    QString mBulletMeshHandle;                              //!< The handle to the bullet's mesh.
};

}

#endif