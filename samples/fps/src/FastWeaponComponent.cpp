#include <FastWeaponComponent.hpp>

FastWeaponComponent::FastWeaponComponent(const QString& name)
    : RaycastComponent(name),
      mPower(0.0f) {}

void FastWeaponComponent::OnCheck(Ogre::Vector3 start, Ogre::Vector3 end) {}

void FastWeaponComponent::OnHit(dt::PhysicsBodyComponent* hit) {
    btVector3 impulse = BtOgre::Convert::toBullet(hit->GetNode()->GetPosition() - GetNode()->GetPosition());
    impulse.normalize();
    
    hit->ApplyCentralImpulse(impulse * mPower);
}

void FastWeaponComponent::SetPower(float power) {
    mPower = power;
}

float FastWeaponComponent::GetPower() {
    return mPower;
}