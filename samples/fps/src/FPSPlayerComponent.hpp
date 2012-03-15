#ifndef DUCTTAPE_SAMPLE_FPS_FPSPLAYERCOMPONENT
#define DUCTTAPE_SAMPLE_FPS_FPSPLAYERCOMPONENT

#include "Weapon.hpp"

#include <Config.hpp>

#include <Logic/AdvancedPlayerComponent.hpp>
#include <Logic/RaycastComponent.hpp>

#include <vector>

#include <QString>

/**
  * A sample fps controller.
  */
class FPSPlayerComponent : public dt::AdvancedPlayerComponent {
    Q_OBJECT
public:
    FPSPlayerComponent(uint16_t weapon_num, const QString name = "");

    void onInitialize();

    void onDeinitialize();

    uint16_t getWeaponNumber() const;

    const Weapon* getWeapon(uint16_t weapon_type) const;

    const Weapon* getWeaponInUse() const;

    const std::vector<Weapon*>& getAllWeapons() const;

public:
    void addWeapon(Weapon* weapon);

    void changeWeapon(uint16_t weapon_type);

    void removeWeapon(uint16_t weapon_type);

signals:
    void sWeaponChanged(const Weapon* current_weapon);

private:
    void _onMouseTriggered();

private slots:
    void _onKeyDown(dt::InputManager::InputCode input_code, const OIS::EventArg& event);

    void _pickUpWeapon(dt::PhysicsBodyComponent* object);

private:
    uint16_t mWeaponNum;
    Weapon* mWeaponInUse;
    std::vector<Weapon*> mWeapons;
    std::shared_ptr<dt::RaycastComponent> mGrabber;
};

#endif
