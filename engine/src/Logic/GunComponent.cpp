
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include <Logic/GunComponent.hpp>
#include <Physics/PhysicsWorld.hpp>
#include <Physics/PhysicsBodyComponent.hpp>
#include <Scene/Scene.hpp>

#include <BtOgreGP.h>

namespace dt {

GunComponent::GunComponent(const QString& name)
    : Component(name),
      mRaycastCallback(nullptr),
      mStart(Ogre::Vector3(0.0f, 0.0f, 0.0f)),
      mEnd(Ogre::Vector3(0.0f, 0.0f, -10.0f)),
      mUseRaycast(true) {}

void GunComponent::OnCreate() {
    btVector3 start, end;
    start = BtOgre::Convert::toBullet(GetNode()->GetRotation() * mStart);
    end = BtOgre::Convert::toBullet(GetNode()->GetRotation() * mEnd);

    mRaycastCallback = new btCollisionWorld::ClosestRayResultCallback(start, end);
}

void GunComponent::OnDestroy() {
    delete mRaycastCallback;
}

Node* GunComponent::Use() {
    btVector3 start, end;
    start = BtOgre::Convert::toBullet(GetNode()->GetRotation() * mStart + GetNode()->GetPosition());
    end = BtOgre::Convert::toBullet(GetNode()->GetRotation() * mEnd + GetNode()->GetPosition());

    GetNode()->GetScene()->GetPhysicsWorld()->GetBulletWorld()->rayTest(start, end, *mRaycastCallback);

    if(mRaycastCallback->hasHit()) {
        btCollisionObject* collision_object = mRaycastCallback->m_collisionObject;
        PhysicsBodyComponent* hit_object = static_cast<PhysicsBodyComponent*>(collision_object->getUserPointer());
        
        return hit_object->GetNode();
    }
    else {
        return nullptr;
    }
}

}