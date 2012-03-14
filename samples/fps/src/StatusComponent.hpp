#ifndef DUCTTAPE_SAMPLE_FPS_STATUSCOMPONENT
#define DUCTTAPE_SAMPLE_FPS_STATUSCOMPONENT

#include <Config.hpp>

#include <Scene/Component.hpp>

#include <QString>

/**
  * A component for recording the character's status like HP.
  */
class StatusComponent : public dt::Component {
    Q_OBJECT
public:
    StatusComponent(uint16_t initial_health, uint16_t max_health);

    uint16_t getHealth();

    void setHealth(uint16_t health);

    uint16_t getMaxHealth();

    void setMaxHealth(uint16_t max_health);

signals:
    void sDeath(const QString name);

    void sHealthChanged(uint16_t previous_health, uint16_t current_health);

public:
    static const QString NAME;

private:
    uint16_t mHealth;
    uint16_t mMaxHealth;
};

#endif
