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

    void OnInitialize();

    void OnDeinitialize();

    uint16_t GetWeaponNumber() const;

    const Weapon* GetWeapon(uint16_t weapon_type) const;

    const Weapon* GetWeaponInUse() const;

    const std::vector<Weapon*>& GetAllWeapons() const;

public:
    void AddWeapon(Weapon* weapon);

    void ChangeWeapon(uint16_t weapon_type);

    void RemoveWeapon(uint16_t weapon_type);

signals:
    void sWeaponChanged(const Weapon* current_weapon);

private:
    void _OnMouseTriggered();

private slots:
    void _OnKeyDown(const OIS::KeyEvent& event);

    void _PickUpWeapon(dt::PhysicsBodyComponent* object);

private:
    uint16_t mWeaponNum;
    Weapon* mWeaponInUse;
    std::vector<Weapon*> mWeapons;
    dt::RaycastComponent* mGrabber;
};

#endif
