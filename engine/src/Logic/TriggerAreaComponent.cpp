#include <Logic/TriggerAreaComponent.hpp>
#include <Scene/Node.hpp>
#include <Scene/Scene.hpp>
#include <Scene/Component.hpp>

namespace dt
{

TriggerAreaComponent::TriggerAreaComponent(btCollisionShape* areaShape,
                                           const QString& name) : 
    Component(name),
    mArea(areaShape) { }

void TriggerAreaComponent::OnUpdate(double time_diff) {
    if(!IsEnabled()) {
        return;
    }

    // TODO: Update only on node position change
    btTransform translation;
    translation.setOrigin(BtOgre::Convert::toBullet(GetNode()->GetPosition()));
    mObject->setWorldTransform(translation);

    for(uint32_t i = 0; i < mObject->getNumOverlappingObjects(); ++i)
    {
        Component* collidingComponent = static_cast<Component*>(mObject->getOverlappingObject(i)->getUserPointer());
        emit Triggered(this, collidingComponent);
    }
}

void TriggerAreaComponent::OnCreate() {
    mObject.reset(new btGhostObject());
    mObject->setCollisionShape(mArea.get());
    mObject->setCollisionFlags(mObject->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
    GetNode()->GetScene()->GetPhysicsWorld()->GetBulletWorld()->addCollisionObject(mObject.get());
}

void TriggerAreaComponent::SetAreaShape(btCollisionShape* areaShape)
{
    mArea.reset(areaShape);
    mObject->setCollisionShape(mArea.get());
}

void TriggerAreaComponent::OnDestroy() {}
void TriggerAreaComponent::OnEnable(){}
void TriggerAreaComponent::OnDisable(){}

}