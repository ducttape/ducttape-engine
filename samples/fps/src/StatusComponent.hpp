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
    StatusComponent(int initial_health, int max_health);

    int GetHealth();

    void SetHealth(int health);

    int GetMaxHealth();

    void SetMaxHealth(int max_health);

signals:
    void sDeath(const QString name);

public:
    static const QString NAME;

private:
    int mHealth;
    int mMaxHealth;
};