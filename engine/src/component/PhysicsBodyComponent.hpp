
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_COMPONENT_PHYSICSBODYCOMPONENT
#define DUCTTAPE_ENGINE_COMPONENT_PHYSICSBODYCOMPONENT

#include <bullet/btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionShapes/btBox2dShape.h>
#include <BulletCollision/CollisionShapes/btConvex2dShape.h>

#include "component/Component.hpp"
#include "event/FreezePhysicsEvent.hpp"
#include "event/UnfreezePhysicsEvent.hpp"
#include "event/ChangeGravityEvent.hpp"

namespace dt {

/**
  * A component making the node physical.
  */
class PhysicsBodyComponent : public Component {
public:
    /**
      * Advanced constructor.
      * @param name The name of the Component.
      * @see Component
      */
    PhysicsBodyComponent(const std::string& name = "");

    virtual void HandleEvent(std::shared_ptr<Event> e);

    void OnCreate();
    void OnDestroy();
    void OnUpdate(double time_diff);


private:
	btCollisionShape* mCollisionShape;
	btRigidBody* mBody;
	btDefaultMotionState* mMotionState;
};

}

#endif
