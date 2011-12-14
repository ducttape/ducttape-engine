#include "SlowWeaponComponent.hpp"
#include "StatusComponent.hpp"

SlowWeaponComponent::SlowWeaponComponent(const QString& bullet_handle, const QString& name)
    : CollisionComponent(bullet_handle, name),
      mPower(0.0f) {}

void SlowWeaponComponent::SetPower(float power) {
    mPower = power;
}

float SlowWeaponComponent::GetPower() {
    return mPower;
}

int SlowWeaponComponent::GetWeaponType() {
    return 2;
}

void SlowWeaponComponent::OnCreate() {
    if(!QObject::connect(this, SIGNAL(sHit(dt::PhysicsBodyComponent*)), 
        this, SLOT(OnHit(dt::PhysicsBodyComponent*)))) {
            dt::Logger::Get().Error("Cannot connect the sHit signal with the OnHit slot.");
    }
}

void SlowWeaponComponent::OnHit(dt::PhysicsBodyComponent* hit) {
    StatusComponent* status = hit->GetNode()->FindComponent<StatusComponent>(StatusComponent::NAME);
    if(status != nullptr) {
        status->SetHealth(status->GetHealth() - mPower);
    }
}