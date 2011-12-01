#include "SlowWeaponComponent.hpp"

SlowWeaponComponent::SlowWeaponComponent(const QString& bullet_handle, const QString& name)
    : CollisionComponent(bullet_handle, name),
      mPower(0.0f) {}

void SlowWeaponComponent::SetPower(float power) {
    mPower = power;
}

float SlowWeaponComponent::GetPower() {
    return mPower;
}