
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_COMPONENT_PHYSICSBODYCOMPONENT
#define DUCTTAPE_ENGINE_COMPONENT_PHYSICSBODYCOMPONENT

#include <btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionShapes/btBox2dShape.h>
#include <BulletCollision/CollisionShapes/btConvex2dShape.h>

#include <BtOgrePG.h>
#include <BtOgreGP.h>
#include <BtOgreExtras.h>

#include "Config.hpp"
#include "component/Component.hpp"
#include "component/MeshComponent.hpp"
#include "scene/Node.hpp"
#include "physics/PhysicsManager.hpp"

namespace dt {

/**
  * A component making the node physical.
  */
class DUCTTAPE_API PhysicsBodyComponent : public Component {
public:
    /**
      * Advanced constructor.
      * @param name The name of the Component.
      * @see Component
      */
    PhysicsBodyComponent(const std::string& mesh_component_name,
                         const std::string& name = "");

    virtual void HandleEvent(std::shared_ptr<Event> e);

    void OnCreate();
    void OnDestroy();
    void OnEnable();
    void OnDisable();
    void OnUpdate(double time_diff);

    /**
      * Sets the mass of the physics body.
      * @param mass The new mass.
      */
    void SetMass(btScalar mass);

private:
    std::string mMeshComponentName;
	btCollisionShape* mCollisionShape;
	btRigidBody* mBody;
    BtOgre::RigidBodyState* mMotionState;
};

}

#endif
