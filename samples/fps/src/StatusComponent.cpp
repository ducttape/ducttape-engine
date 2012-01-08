#include "StatusComponent.hpp"

#include <Scene/Node.hpp>

StatusComponent::StatusComponent(unsigned initial_health, unsigned max_health)
    : Component(NAME),
      mHealth(initial_health),
      mMaxHealth(max_health) {}

unsigned StatusComponent::GetHealth() {
    return mHealth;
}

void StatusComponent::SetHealth(unsigned health) {
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

unsigned StatusComponent::GetMaxHealth() {
    return mMaxHealth;
}

void StatusComponent::SetMaxHealth(unsigned max_health) {
    if(this->IsEnabled())
        mMaxHealth = max_health;
}

const QString StatusComponent::NAME = "status";