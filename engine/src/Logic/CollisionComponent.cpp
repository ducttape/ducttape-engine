
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include <Logic/CollisionComponent.hpp>
#include <Graphics/MeshComponent.hpp>
#include <Scene/Node.hpp>
#include <Scene/Scene.hpp>
#include <Utils/Utils.hpp>

namespace dt {

CollisionComponent::CollisionComponent(const QString& bullet_handle, const QString& name)
    : InteractionComponent(name),
      mBulletMeshHandle(bullet_handle),
      mInitialPower(0.0f) {}

void CollisionComponent::OnCheck() {
    btVector3 start, end, impulse;
    start = BtOgre::Convert::toBullet(GetNode()->GetRotation() * Ogre::Vector3(0.0, 0.0, - mOffset) + GetNode()->GetPosition());
    end = BtOgre::Convert::toBullet(GetNode()->GetRotation() * Ogre::Vector3(0.0, 0.0, - mRange) + GetNode()->GetPosition());
    impulse = end - start;
    impulse.normalize();

    emit sCheck(BtOgre::Convert::toOgre(start), BtOgre::Convert::toOgre(end));

    Node* bullet = GetNode()->GetScene()->AddChildNode(new Node(QString(Utils::AutoId())));

    bullet->AddComponent<MeshComponent>(new MeshComponent(mBulletMeshHandle, "", "bullet"));
    bullet->SetPosition(BtOgre::Convert::toOgre(start), Node::SCENE);
    PhysicsBodyComponent* bullet_body = bullet->AddComponent<PhysicsBodyComponent>(new PhysicsBodyComponent("bullet", "bullet_body"));
    bullet_body->SetMass(1.0);

    if(!QObject::connect(bullet_body, SIGNAL(Collided(dt::PhysicsBodyComponent*, dt::PhysicsBodyComponent*)), 
        this, SLOT(OnHit(dt::PhysicsBodyComponent*, dt::PhysicsBodyComponent*)), Qt::DirectConnection)) {
            Logger::Get().Error("Cannot connect the bullet's collided signal with the OnHit slot.");
    }

    bullet_body->ApplyCentralImpulse(impulse * mInitialPower);
}

float CollisionComponent::GetInitialPower() {
    return mInitialPower;
}

void CollisionComponent::SetInitialPower(float power) {
    mInitialPower = power;
}

void CollisionComponent::OnHit(PhysicsBodyComponent* hit, PhysicsBodyComponent* bullet) {
    Node* node = bullet->GetNode();
    node->Kill();

    emit sHit(hit);
}

}