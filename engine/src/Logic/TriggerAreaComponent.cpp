#include <Logic/TriggerAreaComponent.hpp>
#include <Scene/Node.hpp>
#include <Scene/Scene.hpp>
#include <Scene/Component.hpp>

namespace dt
{

TriggerAreaComponent::TriggerAreaComponent(btCollisionShape* areaShape,
                                           const QString name) : 
    Component(name),
    mArea(areaShape) { }

void TriggerAreaComponent::onUpdate(double time_diff) {
    if(!isEnabled()) {
        return;
    }

    // TODO: Update only on node position change
    btTransform translation;
    translation.setOrigin(BtOgre::Convert::toBullet(getNode()->getPosition()));
    mObject->setWorldTransform(translation);

    for(uint32_t i = 0; i < mObject->getNumOverlappingObjects(); ++i)
    {
        Component* collidingComponent = static_cast<Component*>(mObject->getOverlappingObject(i)->getUserPointer());
        emit triggered(this, collidingComponent);
    }
}

void TriggerAreaComponent::onCreate() {
    mObject.reset(new btGhostObject());
    mObject->setCollisionShape(mArea.get());
    mObject->setCollisionFlags(mObject->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
    getNode()->getScene()->getPhysicsWorld()->getBulletWorld()->addCollisionObject(mObject.get());
}

void TriggerAreaComponent::setAreaShape(btCollisionShape* areaShape)
{
    mArea.reset(areaShape);
    mObject->setCollisionShape(mArea.get());
}

void TriggerAreaComponent::onDestroy() {}
void TriggerAreaComponent::onEnable(){}
void TriggerAreaComponent::onDisable(){}

}