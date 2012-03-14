
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_COMPONENT_PHYSICSBODYCOMPONENT
#define DUCTTAPE_ENGINE_COMPONENT_PHYSICSBODYCOMPONENT

#include <Config.hpp>

#include <Scene/Component.hpp>
#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>

#include <OgreVector3.h>
#include <BtOgrePG.h>

#include <QString>

#include <memory>

namespace dt {

/**
  * A component making the node physical.
  */
class DUCTTAPE_API PhysicsBodyComponent : public Component {
    Q_OBJECT
public:
    DT_SERIALIZABLE(PhysicsBodyComponent)
    /**
      * The type of collision shape that is constructed from the mesh.
      */
    enum CollisionShapeType {
        BOX,        //!< A box
        CONVEX,     //!< A convec shape
        SPHERE,     //!< A sphere
        CYLINDER,   //!< A cylinder
        TRIMESH     //!< The exact triangles of the mesh as collision shape
    };

    /**
      * Advanced constructor.
      * @param mesh_component_name The name of the MeshComponent this
      * PhysicsBodyComponent will use for its shape.
      * @param name The name of the Component.
      * @see MeshComponent
      * @see Component
      */
    PhysicsBodyComponent(const QString mesh_component_name,
                         const QString name = "", CollisionShapeType collision_shape_type = CONVEX, btScalar mass = 5.0f);

    void onInitialize();
    void onDeinitialize();
    void onEnable();
    void onDisable();
    void onUpdate(double time_diff);

    /**
      * Called when the PhysicsBodyComponent collides with another one.
      * @param other_body The pointer to another PhysicsBodyComponent which this PhysicsBodyComponent collided with
      */
    void onCollide(PhysicsBodyComponent* other_body);

    /**
      * Returns Bullet's RigidBody for this PhysicsBodyComponent.
      * @returns Bullet's RigidBody for this PhysicsBodyComponent.
      */
    btRigidBody* getRigidBody();

    /**
      * Applies an impulse(a short-term force) to the center.
      * @param impulse The impulse to apply.
      */
    void applyCentralImpulse(const btVector3& impulse);

    void applyCentralImpulse(float x, float y, float z);

    /**
      * Gives the center a force.
      * @param force The force to give.
      */
    void setCentralForce(const btVector3& force);

    void setCentralForce(float x, float y, float z);

    /**
      * Gives the PhysicsBodyComponent a torque to make it rotate.
      * @param torque The torque to give.
      */
    void setTorque(const btVector3& torque);

    void setTorque(float x, float y, float z);

    /**
      * Sets the collision mask.
      * @param collision_mask The collision mask to set.
      */
    void setCollisionMask(uint16_t collision_mask);

    /**
      * Sets the collision group.
      * @param collision_group The collision group to set.
      */
    void setCollisionGroup(uint16_t collision_group);

    /**
      * Gets the force given to the center.
      * @returns The force given to the center.
      */
    const btVector3 getCentralForce() const;

    /**
      * Gets the torque given to the PhysicsBodyComponent.
      * @returns The torque given to the PhysicsBodyComponent.
      */
    const btVector3 getTorque() const;

    /**
      * Sets movement restriction. This can be used to restrict a body's movement to 1 or 2 dimensions.
      * @param restriction The restriction in x, y, z vector format where x, y, z are either 0
      * to disallow movement on that plane or 1 to allow it.
      */
    void setRestrictMovement(const btVector3& restriction);
    void setRestrictMovement(float x, float y, float z);

    /**
      * Sets rotation restriction. This can be used to restrict a body's rotation to 1 or 2 dimensions.
      * @param restriction The restriction in x, y, z vector format where x, y, z are either 0
      * to disallow movement on that plane or 1 to allow it.
      */
    void setRestrictRotation(const btVector3& restriction);
    void setRestrictRotation(float x, float y, float z);

    /**
      * Sets the gravity of the physics body.
      * @param gravity The new gravity.
      */
    void setGravity(const btVector3& gravity);
    void setGravity(float x, float y, float z);
    
    /**
      * Sets the friction.
      * @param friction The new friction.
      */
    void SetFriction(double friction);

    /**
      * Gets the friction.
      * @returns The friction.
      */
    double GetFriction() const;

    /**
      * Prevents a body from sleeping when it rests. This decreases performance
      * if a large numbers of bodies are active at the same time. Normally, a body
      * would go to sleep if it doesn't move.
      * @param disabled Whether to disable sleep
      */
    void disableSleep(bool disabled);

    void setDampingAmount(btScalar linear_damping, btScalar angular_damping);
    //void SetCollisionShapeType(CollisionShapeType type);

    /**
      * Sets the mass of the physics body.
      * @param mass The new mass.
      */
    void setMass(btScalar mass);

    void activate();

signals:
    void collided(dt::PhysicsBodyComponent* other_body, dt::PhysicsBodyComponent* this_body);

private:
    QString mMeshComponentName;             //!< The name of the mesh component to create the collision shape from.
    btCollisionShape* mCollisionShape;      //!< The bullet collision shape.
    CollisionShapeType mCollisionShapeType; //!< The type of collision shape.
    btRigidBody* mBody;                     //!< The bullet rigid body.
    BtOgre::RigidBodyState* mMotionState;   //!< The motion state of the physics body.
    btVector3 mCentralForce;
    btVector3 mTorque;
    uint16_t mCollisionMask;
    uint16_t mCollisionGroup;
    bool mCollisionMaskInUse;
    btScalar mMass;

};

}

#endif
