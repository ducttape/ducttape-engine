
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
  * A component using collision for interacting with other objects in the scene.
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

    /*
     * Called when Check() is called. It will emit a hit signal. You can just connect it with a slot or just override it.
     */
    virtual void OnCheck(Ogre::Vector3 start, Ogre::Vector3 end);

    /**
      * Check if there's any objects hit by the ray.
      * @see InteractionComponent
      */
    void Check();

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

    /**
      * Sets the initial bullet power.
      * @param power The power to set.
      */
    void SetInitialPower(float power);

    /**
      * Gets the initial bullet power.
      * @returns The initial bullet power.
      */
    float GetInitialPower();

public slots:
     /**
      * Called when the thrown physics body hits an object. It will emit a hit signal. You can just connect it with a slot or just override it.
      * @param hit The hit object.
      */
    virtual void OnHit(PhysicsBodyComponent* hit, PhysicsBodyComponent* bullet);

signals:
    void sHit(PhysicsBodyComponent* hit);
    void sCheck(Ogre::Vector3 start, Ogre::Vector3 end);

private:
    QString mBulletMeshHandle;
    float mInitialPower;
};

}

#endif