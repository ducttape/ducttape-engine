
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
      mBulletMeshHandle(bullet_handle) {}

void CollisionComponent::OnCheck(const btVector3& start, const btVector3& end) {
    btVector3 impulse;
    impulse = end - start;
    impulse.normalize();

    auto id = Utils::AutoId();
    QString name = QString("bullet") + Utils::ToString(id);

    Node* bullet = GetNode()->GetScene()->AddChildNode(new Node(QString(id)));

    bullet->AddComponent<MeshComponent>(new MeshComponent(mBulletMeshHandle, "", name));
    bullet->SetPosition(BtOgre::Convert::toOgre(start), Node::SCENE);
    PhysicsBodyComponent* bullet_body = bullet->AddComponent<PhysicsBodyComponent>(new PhysicsBodyComponent(name, "bullet_body"));
    bullet_body->SetMass(1.0);

    if(!QObject::connect(bullet_body, SIGNAL(Collided(dt::PhysicsBodyComponent*, dt::PhysicsBodyComponent*)), 
        this, SLOT(OnHit(dt::PhysicsBodyComponent*, dt::PhysicsBodyComponent*)), Qt::DirectConnection)) {
            Logger::Get().Error("Cannot connect the bullet's collided signal with the OnHit slot.");
    }

    bullet_body->ApplyCentralImpulse(impulse * mRange);
}

void CollisionComponent::OnHit(PhysicsBodyComponent* hit, PhysicsBodyComponent* bullet) {
    Node* node = bullet->GetNode();
    node->Kill();

    emit sHit(hit);
}

void CollisionComponent::OnCreate() {
    //Preload the bullet mesh.
    Node* bullet = this->GetNode()->GetScene()->AddChildNode(new Node("preload_bullet"));
    bullet->AddComponent(new MeshComponent(mBulletMeshHandle, "", "bullet"));
    bullet->SetPosition(0, -100, 0, Node::SCENE);

    bullet->Kill();
}
}