
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include <Logic/RaycastComponent.hpp>
#include <Physics/PhysicsWorld.hpp>
#include <Physics/PhysicsBodyComponent.hpp>
#include <Scene/Scene.hpp>

#include <BtOgreGP.h>

namespace dt {

RaycastComponent::RaycastComponent(const QString& name)
    : InteractionComponent(name) {}

void RaycastComponent::OnCheck() {
    btVector3 start, end;
    start = BtOgre::Convert::toBullet(GetNode()->GetRotation() * Ogre::Vector3(0.0, 0.0, - mOffset) + GetNode()->GetPosition());
    end = BtOgre::Convert::toBullet(GetNode()->GetRotation() * Ogre::Vector3(0.0, 0.0, - mRange) + GetNode()->GetPosition());

    emit sCheck(BtOgre::Convert::toOgre(start), BtOgre::Convert::toOgre(end));

    btCollisionWorld::ClosestRayResultCallback raycast_callback(start, end);

    GetNode()->GetScene()->GetPhysicsWorld()->GetBulletWorld()->rayTest(start, end, raycast_callback);

    if(raycast_callback.hasHit()) {
        btCollisionObject* collision_object = raycast_callback.m_collisionObject;
        PhysicsBodyComponent* hit_object = static_cast<PhysicsBodyComponent*>(collision_object->getUserPointer());
        
        emit sHit(hit_object);
    }
}

}