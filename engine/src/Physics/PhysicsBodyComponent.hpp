
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
    PhysicsBodyComponent(const QString& mesh_component_name,
                         const QString& name = "");

    void OnCreate();
    void OnDestroy();
    void OnEnable();
    void OnDisable();
    void OnUpdate(double time_diff);
    void OnCollide(PhysicsBodyComponent* other_body);
    btRigidBody* GetRigidBody();
    void ApplyCentralImpulse(const btVector3& impulse);
    void SetCentralForce(const btVector3& force);
    void SetTorque(const btVector3& torque);
    void SetCollisionMask(uint16_t collision_mask);
    void SetCollisionGroup(uint16_t collision_group);
    const btVector3 GetCentralForce() const;
    const btVector3 GetTorque() const;

    /**
      * Sets movement restriction. This can be used to restrict a body's movement to 1 or 2 dimensions.
      * @param restriction The restriction in x, y, z vector format where x, y, z are either 0
      * to disallow movement on that plane or 1 to allow it.
      */
    void SetRestrictMovement(const btVector3& restriction);

    /**
      * Sets rotation restriction. This can be used to restrict a body's rotation to 1 or 2 dimensions.
      * @param restriction The restriction in x, y, z vector format where x, y, z are either 0
      * to disallow movement on that plane or 1 to allow it.
      */
    void SetRestrictRotation(const btVector3& restriction);

    /**
      * Sets the gravity of the physics body.
      * @param gravity The new gravity.
      */
    void SetGravity(const btVector3& gravity);
    
    /**
      * Prevents a body from sleeping when it rests. This decreases performance
      * if a large numbers of bodies are active at the same time. Normally, a body
      * would go to sleep if it doesn't move.
      * @param disabled Whether to disable sleep
      */
    void DisableSleep(bool disabled);

    void SetDampingAmount(btScalar linear_damping, btScalar angular_damping);
    void SetCollisionShapeType(CollisionShapeType type);

    /**
      * Sets the mass of the physics body.
      * @param mass The new mass.
      */
    void SetMass(btScalar mass);

signals:
    void Collided(dt::PhysicsBodyComponent* other_body);

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

};

}

#endif
