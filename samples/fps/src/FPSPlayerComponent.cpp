#include "FPSPlayerComponent.hpp"

#include <Logic/InteractionComponent.hpp>
#include <Scene/Scene.hpp>
#include <Input/InputManager.hpp>
#include <Utils/Logger.hpp>

#include <OgreProcedural.h>

FPSPlayerComponent::FPSPlayerComponent(uint16_t weapon_num, const QString& name)
    : AdvancedPlayerComponent(name),
      mWeaponNum(weapon_num),
      mWeaponInUse(nullptr),
      mGrabber(nullptr) {}

void FPSPlayerComponent::OnCreate() {
    AdvancedPlayerComponent::OnCreate();

    for(uint16_t i = 0; i < mWeaponNum; ++i) {
        mWeapons.push_back(nullptr);
    }

    mGrabber = this->GetNode()->AddComponent<dt::RaycastComponent>(new dt::RaycastComponent("grabber"));
    mGrabber->SetRange(3.0f);
    
    if(!QObject::connect(mGrabber, SIGNAL(sHit(dt::PhysicsBodyComponent*)), 
        this, SLOT(_PickUpWeapon(dt::PhysicsBodyComponent*)))) {
            dt::Logger::Get().Error("Cannot connect the grabber's sHit signal with FPSPlayerComponent " + 
                GetName() + "'s _PickUpWeapon slot.");
    }
}

void FPSPlayerComponent::OnDestroy() {
    AdvancedPlayerComponent::OnDestroy();
}

void FPSPlayerComponent::AddWeapon(Weapon* weapon) {
    uint16_t index = weapon->GetType();

    if(index < mWeaponNum) {
        if(mWeapons[index] != nullptr)
            this->RemoveWeapon(index);

        weapon->EnablePhysicsBody(false);
        weapon->SetParent(this->GetNode());
        weapon->SetRotation(Ogre::Quaternion::IDENTITY);
        weapon->SetPosition(0.5f, -0.5f, -1.0f);
        mWeapons[index] = weapon;

        if(mWeaponInUse == nullptr) {
            this->ChangeWeapon(index);
        }
        else {
            weapon->Disable();
        }
    }
}

void FPSPlayerComponent::ChangeWeapon(uint16_t weapon_type) {
    if(mWeaponNum > weapon_type && mWeapons[weapon_type] != nullptr && mWeaponInUse != mWeapons[weapon_type]) {
        if(mWeaponInUse != nullptr)
            mWeaponInUse->Disable();

        mWeaponInUse = mWeapons[weapon_type];
        mWeaponInUse->Enable();

        emit sWeaponChanged(mWeaponInUse);
    }      
}

uint16_t FPSPlayerComponent::GetWeaponNumber() const {
    return mWeaponNum;
}

const Weapon* FPSPlayerComponent::GetWeapon(uint16_t weapon_type) const {
    return mWeapons[weapon_type];
}

const std::vector<Weapon*>& FPSPlayerComponent::GetAllWeapons() const {
    return mWeapons;
}

void FPSPlayerComponent::_OnKeyDown(const OIS::KeyEvent& event) {
    switch(event.key) {
    case OIS::KC_1:
        ChangeWeapon(0);
        break;
    case OIS::KC_2:
        ChangeWeapon(1);
        break;
    case OIS::KC_3:
        ChangeWeapon(2);
        break;
    case OIS::KC_4:
        ChangeWeapon(3);
        break;
    case OIS::KC_5:
        ChangeWeapon(4);
        break;
    case OIS::KC_6:
        ChangeWeapon(5);
        break;
    case OIS::KC_7:
        ChangeWeapon(6);
        break;
    case OIS::KC_8:
        ChangeWeapon(7);
        break;
    case OIS::KC_9:
        ChangeWeapon(8);
        break;
    case OIS::KC_G:
        if(mWeaponInUse != nullptr)
            RemoveWeapon(mWeaponInUse->GetType());
        break;
    case OIS::KC_R:
        if(mWeaponInUse != nullptr)
            mWeaponInUse->Reload();
        break;
    case OIS::KC_E:
        mGrabber->Check();
        break;
    default:
        return;
    }
}

void FPSPlayerComponent::RemoveWeapon(uint16_t weapon_type) {
    if(mWeaponNum >= weapon_type && mWeapons[weapon_type] != nullptr) {
        if(mWeaponInUse == mWeapons[weapon_type])
            mWeaponInUse = nullptr;

        mWeapons[weapon_type]->SetPosition(0.0f, 0.0f, -3.0f);
        Ogre::Vector3 pos = mWeapons[weapon_type]->GetPosition(dt::Node::SCENE);
        mWeapons[weapon_type]->SetParent(this->GetNode()->GetScene());
        mWeapons[weapon_type]->SetPosition(pos, dt::Node::SCENE);
        mWeapons[weapon_type]->EnablePhysicsBody(true);
        mWeapons[weapon_type]->Enable();
        mWeapons[weapon_type] = nullptr;
    }
}

void FPSPlayerComponent::_OnMouseTriggered() {
    if(mIsLeftMouseDown && mWeaponInUse != nullptr)
        mWeaponInUse->Fire();
}

void FPSPlayerComponent::_PickUpWeapon(dt::PhysicsBodyComponent* object) {
    Weapon* weapon = dynamic_cast<Weapon*>(object->GetNode());

    if(weapon != nullptr) {
        AddWeapon(weapon);
    }
}

const Weapon* FPSPlayerComponent::GetWeaponInUse() const {
    return mWeaponInUse;
}
