#include "FPSPlayerComponent.hpp"

#include <Logic/InteractionComponent.hpp>
#include <Scene/Scene.hpp>
#include <Input/InputManager.hpp>
#include <Utils/Logger.hpp>

#include <OgreProcedural.h>

FPSPlayerComponent::FPSPlayerComponent(uint16_t weapon_num, const QString name)
    : AdvancedPlayerComponent(name),
      mWeaponNum(weapon_num),
      mWeaponInUse(nullptr),
      mGrabber(nullptr) {}

void FPSPlayerComponent::onInitialize() {
    AdvancedPlayerComponent::onInitialize();

    for(uint16_t i = 0; i < mWeaponNum; ++i) {
        mWeapons.push_back(nullptr);
    }

    mGrabber = this->getNode()->addComponent<dt::RaycastComponent>(new dt::RaycastComponent("grabber"));
    mGrabber->setRange(3.0f);
    
    if(!QObject::connect(mGrabber.get(), SIGNAL(sHit(dt::PhysicsBodyComponent*)), 
                         this,     SLOT(_pickUpWeapon(dt::PhysicsBodyComponent*)))) {
            dt::Logger::get().error("Cannot connect the grabber's signal sHit with FPSPlayerComponent " +
                getName() + "'s slot _PickUpWeapon.");
    }

    if(!QObject::connect(dt::InputManager::get(), SIGNAL(sPressed(dt::InputManager::InputCode, const OIS::EventArg&)),
                         this, SLOT(_onKeyDown(dt::InputManager::InputCode, const OIS::EventArg&)))) {
            dt::Logger::get().error("Cannot connect signal sPressed with " + getName()
                + "'s input handling slot.");
    }
}

void FPSPlayerComponent::onDeinitialize() {
    AdvancedPlayerComponent::onDeinitialize();
}

void FPSPlayerComponent::addWeapon(Weapon* weapon) {
    uint16_t index = weapon->getType();

    if(index < mWeaponNum) {
        if(mWeapons[index] != nullptr)
            this->removeWeapon(index);

        weapon->enablePhysicsBody(false);
        weapon->setParent(this->getNode());
        weapon->setRotation(Ogre::Quaternion::IDENTITY);
        weapon->setPosition(0.5f, -0.5f, -1.0f);
        mWeapons[index] = weapon;

        if(mWeaponInUse == nullptr) {
            this->changeWeapon(index);
        }
        else {
            weapon->disable();
        }
    }
}

void FPSPlayerComponent::changeWeapon(uint16_t weapon_type) {
    if(mWeaponNum > weapon_type && mWeapons[weapon_type] != nullptr && mWeaponInUse != mWeapons[weapon_type]) {
        if(mWeaponInUse != nullptr)
            mWeaponInUse->disable();

        mWeaponInUse = mWeapons[weapon_type];
        mWeaponInUse->enable();

        emit sWeaponChanged(mWeaponInUse);
    }      
}

uint16_t FPSPlayerComponent::getWeaponNumber() const {
    return mWeaponNum;
}

const Weapon* FPSPlayerComponent::getWeapon(uint16_t weapon_type) const {
    return mWeapons[weapon_type];
}

const std::vector<Weapon*>& FPSPlayerComponent::getAllWeapons() const {
    return mWeapons;
}

void FPSPlayerComponent::_onKeyDown(dt::InputManager::InputCode input_code, const OIS::EventArg& event) {
    if(this->isEnabled()) {
        switch(input_code) {
        case dt::InputManager::KC_1:
            changeWeapon(0);
            break;
        case dt::InputManager::KC_2:
            changeWeapon(1);
            break;
        case dt::InputManager::KC_3:
            changeWeapon(2);
            break;
        case dt::InputManager::KC_4:
            changeWeapon(3);
            break;
        case dt::InputManager::KC_5:
            changeWeapon(4);
            break;
        case dt::InputManager::KC_6:
            changeWeapon(5);
            break;
        case dt::InputManager::KC_7:
            changeWeapon(6);
            break;
        case dt::InputManager::KC_8:
            changeWeapon(7);
            break;
        case dt::InputManager::KC_9:
            changeWeapon(8);
            break;
        case dt::InputManager::KC_G:
            if(mWeaponInUse != nullptr)
                removeWeapon(mWeaponInUse->getType());
            break;
        case dt::InputManager::KC_R:
            if(mWeaponInUse != nullptr)
                mWeaponInUse->reload();
            break;
        case dt::InputManager::KC_E:
            mGrabber->check();
            break;
        default:
            return;
        }
    }
}

void FPSPlayerComponent::removeWeapon(uint16_t weapon_type) {
    if(mWeaponNum >= weapon_type && mWeapons[weapon_type] != nullptr) {
        if(mWeaponInUse == mWeapons[weapon_type])
            mWeaponInUse = nullptr;

        mWeapons[weapon_type]->setPosition(0.0f, 0.0f, -3.0f);
        Ogre::Vector3 pos = mWeapons[weapon_type]->getPosition(dt::Node::SCENE);
        mWeapons[weapon_type]->setParent(this->getNode()->getScene());
        mWeapons[weapon_type]->setPosition(pos, dt::Node::SCENE);
        mWeapons[weapon_type]->enablePhysicsBody(true);
        mWeapons[weapon_type]->enable();
        mWeapons[weapon_type] = nullptr;
    }
}

void FPSPlayerComponent::_onMouseTriggered() {
    if(mIsLeftMouseDown && mWeaponInUse != nullptr)
        mWeaponInUse->fire();
}

void FPSPlayerComponent::_pickUpWeapon(dt::PhysicsBodyComponent* object) {
    Weapon* weapon = dynamic_cast<Weapon*>(object->getNode());

    if(weapon != nullptr) {
        addWeapon(weapon);
    }
}

const Weapon* FPSPlayerComponent::getWeaponInUse() const {
    return mWeaponInUse;
}
