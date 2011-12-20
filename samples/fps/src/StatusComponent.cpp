#include "StatusComponent.hpp"

#include <Scene/Node.hpp>

StatusComponent::StatusComponent(int initial_health, int max_health)
    : Component(NAME),
      mHealth(initial_health),
      mMaxHealth(max_health) {}

int StatusComponent::GetHealth() {
    return mHealth;
}

void StatusComponent::SetHealth(int health) {
    if(this->IsEnabled()) {
        if(health <= 0) {
            mHealth = 0;

            emit sDeath(this->GetNode()->GetName());
        }
        else if(health >= mMaxHealth) {
            mHealth = mMaxHealth;
        }
        else {
            mHealth = health;
        }
    }
}

int StatusComponent::GetMaxHealth() {
    return mMaxHealth;
}

void StatusComponent::SetMaxHealth(int max_health) {
    if(this->IsEnabled())
        mMaxHealth = max_health;
}

const QString StatusComponent::NAME = "status";