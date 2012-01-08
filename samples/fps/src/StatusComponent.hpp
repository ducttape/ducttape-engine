#ifndef STATUSCOMPONENT_H
#define STATUSCOMPONENT_H
#pragma once

#include <Config.hpp>

#include <Scene/Component.hpp>

#include <QString>

/**
  * A component for recording the character's status like HP.
  */
class StatusComponent : public dt::Component {
    Q_OBJECT
public:
    StatusComponent(unsigned initial_health, unsigned max_health);

    unsigned GetHealth();

    void SetHealth(unsigned health);

    unsigned GetMaxHealth();

    void SetMaxHealth(unsigned max_health);

signals:
    void sDeath(const QString name);

    void sHealthChanged(unsigned previous_health, unsigned current_health);

public:
    static const QString NAME;

private:
    unsigned mHealth;
    unsigned mMaxHealth;
};

#endif