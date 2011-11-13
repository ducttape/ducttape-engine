
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
    void SetForce(const btVector3& force);
    void SetTorque(const btVector3& torque);
    void HasForce(bool hasforce);
    void HasTorque(bool hastorque);
    void GetForce();
    void GetTorque();
    void SetTwoDimensional(bool twod);
    void SetAngle(const btVector3& angle);
    void SetGravity(const btVector3& gravity);
    void DisableDeactivation(bool disabled);
    void SetDampingAmount(btScalar lindamping, btScalar angdamping);
    
    /**
      * Sets the mass of the physics body.
      * @param mass The new mass.
      */
    void SetMass(btScalar mass);

private:
    QString mMeshComponentName;         //!< The name of the mesh component to create the collision shape from.
    btCollisionShape* mCollisionShape;      //!< The bullet collision shape.
    btRigidBody* mBody;                     //!< The bullet rigid body.
    BtOgre::RigidBodyState* mMotionState;   //!< The motion state of the physics body.
    btVector3 Force;
    btVector3 Torque;
    btVector3 Angle;
    bool hasTorque;
    bool hasForce;
    btScalar lindamping;
    btScalar angdamping;
};

}

#endif
