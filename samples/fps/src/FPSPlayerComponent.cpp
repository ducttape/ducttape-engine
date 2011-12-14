#include "FPSPlayerComponent.hpp"
#include "FastWeaponComponent.hpp"
#include "SlowWeaponComponent.hpp"

#include <Logic/InteractionComponent.hpp>
#include <Input/InputManager.hpp>
#include <Utils/Logger.hpp>

#include <OgreProcedural.h>

FPSPlayerComponent::FPSPlayerComponent(int weapon_num, const QString& name) : 
      AdvancePlayerComponent(name),
      mWeapons(nullptr),
      mWeaponNum(weapon_num) {}

void FPSPlayerComponent::OnCreate() {
    AdvancePlayerComponent::OnCreate();

    mWeapons = new QString[mWeaponNum];
    for(int i = 0 ; i < mWeaponNum ; i++) {
        mWeapons[i] = "";
    }

    if(!QObject::connect(dt::InputManager::Get(), SIGNAL(sKeyPressed(const OIS::KeyEvent&)), 
        this, SLOT(ChangeWeapon(const OIS::KeyEvent&)))) {
            dt::Logger::Get().Error("Cannot connect the key pressed signal with " + GetName()
                + "'s keyboard input handling slot.");
    }
}

void FPSPlayerComponent::OnDestroy() {
    AdvancePlayerComponent::OnDestroy();
    
    if(mWeaponNum > 0) {
        delete[] mWeapons;
    }
}

void FPSPlayerComponent::ChangeWeapon(const OIS::KeyEvent& event) {
    if(event.key == OIS::KC_1 && mWeaponNum > 0) {
        this->SetInteractionComponentName(mWeapons[0]);
    }
    else if(event.key == OIS::KC_2 && mWeaponNum > 1) {
        this->SetInteractionComponentName(mWeapons[1]);
    }
}

/*void FPSPlayerComponent::AddWeapon(IWeapon* weapon) {
    int type = weapon->GetWeaponType();

    if(weapon != nullptr && type >= 0 && type <= mWeaponNum) {
        mWeapons[type] = ((dt::InteractionComponent*)(weapon))->GetName();
        if(!this->GetNode()->HasComponent(mWeapons[type])) {
            this->GetNode()->AddComponent((dt::InteractionComponent*)weapon);
        }
    }
}*/

void FPSPlayerComponent::RemoveWeapon(int type) {
    if(type >= 0 && type <= mWeaponNum) {
        if(this->GetInteractionComponentName() == mWeapons[type]) {
            this->SetInteractionComponentName("");
        }
        this->GetNode()->RemoveComponent(mWeapons[type]);
        mWeapons[type] = "";
    }
}