#include "FastWeaponComponent.hpp"
#include "StatusComponent.hpp"

#include <Utils/Logger.hpp>

FastWeaponComponent::FastWeaponComponent(const QString& name)
    : RaycastComponent(name),
      mPower(0.0f) {}

void FastWeaponComponent::OnCreate() {
    if(!QObject::connect(this, SIGNAL(sHit(dt::PhysicsBodyComponent*)), 
        this, SLOT(OnHit(dt::PhysicsBodyComponent*)))) {
            dt::Logger::Get().Error("Cannot connect the sHit signal with the OnHit slot.");
    }
}

void FastWeaponComponent::OnHit(dt::PhysicsBodyComponent* hit) {
    btVector3 impulse = BtOgre::Convert::toBullet(hit->GetNode()->GetPosition() - GetNode()->GetPosition());
    impulse.normalize();
    hit->Activate();
    hit->ApplyCentralImpulse(impulse * mPower);

    StatusComponent* status = hit->GetNode()->FindComponent<StatusComponent>(StatusComponent::NAME);
    if(status != nullptr) {
        status->SetHealth(status->GetHealth() - mPower);
    }
}

void FastWeaponComponent::SetPower(float power) {
    mPower = power;
}

float FastWeaponComponent::GetPower() {
    return mPower;
}

int FastWeaponComponent::GetWeaponType() {
    return 1;
}