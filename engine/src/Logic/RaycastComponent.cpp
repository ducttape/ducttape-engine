
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
    : InteractionComponent(name),
      mRaycastCallback(nullptr) {}

void RaycastComponent::OnCreate() {
    btVector3 start, end;
    start = BtOgre::Convert::toBullet(GetNode()->GetRotation() * mStart);
    end = BtOgre::Convert::toBullet(GetNode()->GetRotation() * mEnd);

    mRaycastCallback = new btCollisionWorld::ClosestRayResultCallback(start, end);
}

void RaycastComponent::OnDestroy() {
    delete mRaycastCallback;
}

void RaycastComponent::Check() {
    btVector3 start, end;
    start = BtOgre::Convert::toBullet(GetNode()->GetRotation() * mStart + GetNode()->GetPosition());
    end = BtOgre::Convert::toBullet(GetNode()->GetRotation() * mEnd + GetNode()->GetPosition());

    OnCheck(BtOgre::Convert::toOgre(start), BtOgre::Convert::toOgre(end));

    GetNode()->GetScene()->GetPhysicsWorld()->GetBulletWorld()->rayTest(start, end, *mRaycastCallback);

    if(mRaycastCallback->hasHit()) {
        btCollisionObject* collision_object = mRaycastCallback->m_collisionObject;
        PhysicsBodyComponent* hit_object = static_cast<PhysicsBodyComponent*>(collision_object->getUserPointer());
        
        OnHit(hit_object->GetNode());
    }
}

void RaycastComponent::OnCheck(Ogre::Vector3 start, Ogre::Vector3 end) {
    emit sCheck(start, end);
}

void RaycastComponent::OnHit(Node* hit) {
    emit sHit(hit);
}

}