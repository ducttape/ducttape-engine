#include "StatusComponent.hpp"

#include <Scene/Node.hpp>

StatusComponent::StatusComponent(uint16_t initial_health, uint16_t max_health)
    : Component(NAME),
      mHealth(initial_health),
      mMaxHealth(max_health) {}

uint16_t StatusComponent::GetHealth() {
    return mHealth;
}

void StatusComponent::SetHealth(uint16_t health) {
    if(this->IsEnabled() && mHealth != health) {
        if(health >= mMaxHealth) {
            health = mMaxHealth;
        }
        
        //Exchange the two values.
        mHealth ^= health;
        health ^= mHealth;
        mHealth ^= health;
        
        emit sHealthChanged(health, mHealth);
        
        if(mHealth == 0)
            emit sDeath(this->GetNode()->GetName());
    }
}

uint16_t StatusComponent::GetMaxHealth() {
    return mMaxHealth;
}

void StatusComponent::SetMaxHealth(uint16_t max_health) {
    if(this->IsEnabled())
        mMaxHealth = max_health;
}

const QString StatusComponent::NAME = "status";