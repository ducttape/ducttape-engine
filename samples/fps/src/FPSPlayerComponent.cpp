#include "FPSPlayerComponent.hpp"
#include "FastWeaponComponent.hpp"
#include "SlowWeaponComponent.hpp"

#include <Input/InputManager.hpp>
#include <Utils/Logger.hpp>

#include <OgreProcedural.h>

FPSPlayerComponent::FPSPlayerComponent(const QString& name) : 
      AdvancePlayerComponent(name),
      mWeapons(nullptr) {}

void FPSPlayerComponent::OnCreate() {
    AdvancePlayerComponent::OnCreate();

    int weapon_num = 2;
    mWeapons = new QString[weapon_num];
    mWeapons[0] = "fast_weapon";
    mWeapons[1] = "slow_weapon";

    OgreProcedural::SphereGenerator().setRadius(0.5f).setUTile(.5f).realizeMesh("Bullet");
    FastWeaponComponent* fast_weapon = GetNode()->AddComponent(new FastWeaponComponent(mWeapons[0]));
    fast_weapon->SetRange(10.0f);
    fast_weapon->SetPower(1.0f);
    SlowWeaponComponent* slow_weapon = GetNode()->AddComponent(new SlowWeaponComponent("Bullet", mWeapons[1]));
    slow_weapon->SetRange(10.0f);
    slow_weapon->SetOffset(2.1f);          //For not colliding with the player.
    slow_weapon->SetInitialPower(10.0f);
    slow_weapon->SetPower(70.0f);

    if(!QObject::connect(dt::InputManager::Get(), SIGNAL(sKeyPressed(const OIS::KeyEvent&)), 
        this, SLOT(ChangeWeapon(const OIS::KeyEvent&)))) {
            dt::Logger::Get().Error("Cannot connect the key pressed signal with " + GetName()
                + "'s keyboard input handling slot.");
    }

    this->SetInteractionComponentName(mWeapons[0]);
}

void FPSPlayerComponent::OnDestroy() {
    AdvancePlayerComponent::OnDestroy();
    delete[] mWeapons;
}

void FPSPlayerComponent::ChangeWeapon(const OIS::KeyEvent& event) {
    if(event.key == OIS::KC_1) {
        this->SetInteractionComponentName(mWeapons[0]);
    }
    else if(event.key == OIS::KC_2) {
        this->SetInteractionComponentName(mWeapons[1]);
    }
}