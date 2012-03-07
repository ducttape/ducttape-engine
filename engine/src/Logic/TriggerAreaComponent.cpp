#include <Logic/TriggerAreaComponent.hpp>
#include <Scene/Node.hpp>
#include <Scene/Scene.hpp>
#include <Scene/Component.hpp>


namespace dt
{

TriggerAreaComponent::TriggerAreaComponent(btCollisionShape* area,
                                           const QString& name) : 
    Component(name),
    mArea(area) { }

void TriggerAreaComponent::OnUpdate(double time_diff) {

    if(mObject->getNumOverlappingObjects() > 0) {
        emit Triggered(this);
    }
}

void TriggerAreaComponent::OnCreate() {
    mObject = new btGhostObject();
    mObject->setCollisionShape(mArea);
    mObject->setCollisionFlags(mObject->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
    GetNode()->GetScene()->GetPhysicsWorld()->GetBulletWorld()->addCollisionObject(mObject);
}

void TriggerAreaComponent::SetAreaShape(btCollisionShape* area)
{
    mArea = area;
    mObject->setCollisionShape(mArea);
}

void TriggerAreaComponent::OnDestroy() {
    delete mObject;
    delete mArea;
}
void TriggerAreaComponent::OnEnable(){}
void TriggerAreaComponent::OnDisable(){}

}