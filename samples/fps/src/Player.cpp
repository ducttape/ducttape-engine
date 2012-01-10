#include "Player.hpp"

#include "Utils/Logger.hpp"

Player::Player(const QString& name)
    : Hittable(name),
      mController(nullptr),
      mCamera(nullptr),
      mStatus(nullptr),
      mMesh(nullptr),
      mIsControllable(false) {}

void Player::OnInitialize() {
    mController = new FPSPlayerComponent(2, "controller");
    mMesh = new dt::MeshComponent("player", "", "player_mesh");
    mStatus = new StatusComponent(100, 100);
    mCamera = new dt::CameraComponent("main_camera");
    
    //const Weapon* weapon = mController->GetWeaponInUse();

    //this->AddComponent(mMesh);
    this->AddComponent(mStatus);
    this->AddComponent(mCamera)->LookAt(Ogre::Vector3(0, 0, -10));
    this->AddComponent(mController);
    if(mIsControllable)
        mController->Enable();
    else
        mController->Disable();

    /*_RefreshHealth(0, 100);
    if(weapon != nullptr) {
        _OnWeaponChanged(weapon);
    }*/

    /*if(!QObject::connect(mController, SIGNAL(sWeaponChanged(const Weapon*)), 
        this, SLOT(_OnWeaponChanged(const Weapon*)), Qt::DirectConnection)) {
            dt::Logger::Get().Debug(QString("Failed to connect the controller's sWeaponChanged") +
                QString("signal with the player's _OnWeaponChanged slot"));
    }*/
}

void Player::_RefreshAmmo(unsigned current_ammo) {
}

void Player::_RefreshClip(unsigned current_clip) {
}

void Player::_RefreshHealth(unsigned previous_health, unsigned current_health) {
}

void Player::_OnWeaponChanged(const Weapon* current_weapon) {
    if(!QObject::disconnect(this, SLOT(_RefreshAmmo(unsigned))))
        dt::Logger::Get().Debug("Failed to disconnect the player's _RefreshAmmo slot!");
    if(!QObject::disconnect(this, SLOT(_RefreshClip(unsigned))))
        dt::Logger::Get().Debug("Failed to disconnect the player's _RefreshClip slot!");

    if(current_weapon == nullptr) {
        _RefreshClip(0);
        _RefreshAmmo(0);
    }
    else {
        if(!QObject::connect(current_weapon, SIGNAL(sAmmoChanged(unsigned)),
            this, SLOT(_RefreshAmmo(unsigned))))
            dt::Logger::Get().Debug("Failed to connect the new weapon's sAmmoChanged signal!");
        if(!QObject::connect(current_weapon, SIGNAL(sClipChanged(unsigned)),
            this, SLOT(_RefreshClip(unsigned))))
            dt::Logger::Get().Debug("Failed to connect the new weapon's sClipChanged signal!");
       
        _RefreshClip(current_weapon->GetCurrentClip());
        _RefreshAmmo(current_weapon->GetCurrentAmmo());
    }
}

const FPSPlayerComponent* Player::GetController() const {
    return mController;
}

const dt::CameraComponent* Player::GetCamera() const {
    return mCamera;
}

const StatusComponent* Player::GetStatus() const {
    return mStatus;
}

const dt::MeshComponent* Player::GetMesh() const {
    return mMesh;
}

void Player::OnHit(int damage) {
    mStatus->SetHealth(mStatus->GetHealth() - damage);
}

void Player::SetControllable(bool is_controllable) {
    if(is_controllable != mIsControllable) {
        if(mIsControllable = is_controllable)
            mController->Enable();
        else
            mController->Disable();
    }
}

bool Player::IsControllable() const {
    return mIsControllable;
}

void Player::OnEnable() {
    if(!mIsControllable)
        mController->Disable();
}