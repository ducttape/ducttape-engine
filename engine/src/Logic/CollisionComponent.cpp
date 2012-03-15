
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

CollisionComponent::CollisionComponent(const QString bullet_handle, const QString name)
    : InteractionComponent(name),
      mBulletMeshHandle(bullet_handle) {}

void CollisionComponent::onCheck(const btVector3& start, const btVector3& end) {
    btVector3 impulse;
    impulse = end - start;
    impulse.normalize();

    auto id = Utils::autoId();
    QString name = QString("bullet") + Utils::toString(id);

    std::shared_ptr<Node> bullet = getNode()->getScene()->addChildNode(new Node(QString(id)));

    bullet->addComponent<MeshComponent>(new MeshComponent(mBulletMeshHandle, "", name));
    bullet->setPosition(BtOgre::Convert::toOgre(start), Node::SCENE);
    std::shared_ptr<PhysicsBodyComponent> bullet_body = bullet->addComponent<PhysicsBodyComponent>(new PhysicsBodyComponent(name, "bullet_body"));
    bullet_body->setMass(1.0);

    if(!QObject::connect(bullet_body.get(), SIGNAL(Collided(dt::PhysicsBodyComponent*, dt::PhysicsBodyComponent*)),
                         this,        SLOT(OnHit(dt::PhysicsBodyComponent*, dt::PhysicsBodyComponent*)), Qt::DirectConnection)) {
            Logger::get().error("Cannot connect the bullet's collided signal with the OnHit slot.");
    }

    bullet_body->applyCentralImpulse(impulse * mRange);
}

void CollisionComponent::onHit(PhysicsBodyComponent* hit, PhysicsBodyComponent* bullet) {
    Node* node = bullet->getNode();
    node->kill();

    emit sHit(hit);
}

void CollisionComponent::onInitialize() {
    //Preload the bullet mesh.
    std::shared_ptr<Node> bullet = this->getNode()->getScene()->addChildNode(new Node("preload_bullet"));
    bullet->addComponent(new MeshComponent(mBulletMeshHandle, "", "bullet"));
    bullet->setPosition(0, -100, 0, Node::SCENE);

    bullet->kill();
}
}
