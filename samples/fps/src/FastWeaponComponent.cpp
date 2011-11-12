#include <FastWeaponComponent.hpp>

FastWeaponComponent::FastWeaponComponent(const QString& name)
    : RaycastComponent(name) {}

void FastWeaponComponent::OnCheck() {}

void FastWeaponComponent::OnHit(dt::Node* hit) {}