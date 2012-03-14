#include "StatusComponent.hpp"

#include <Scene/Node.hpp>

StatusComponent::StatusComponent(uint16_t initial_health, uint16_t max_health)
    : Component(NAME),
      mHealth(initial_health),
      mMaxHealth(max_health) {}

uint16_t StatusComponent::getHealth() {
    return mHealth;
}

void StatusComponent::setHealth(uint16_t health) {
    if(this->isEnabled() && mHealth != health) {
        if(health >= mMaxHealth) {
            health = mMaxHealth;
        }
        
        //Exchange the two values.
        mHealth ^= health;
        health ^= mHealth;
        mHealth ^= health;
        
        emit sHealthChanged(health, mHealth);
        
        if(mHealth == 0)
            emit sDeath(this->getNode()->getName());
    }
}

uint16_t StatusComponent::getMaxHealth() {
    return mMaxHealth;
}

void StatusComponent::setMaxHealth(uint16_t max_health) {
    if(this->isEnabled())
        mMaxHealth = max_health;
}

const QString StatusComponent::NAME = "status";
