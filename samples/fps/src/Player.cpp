#include "Player.hpp"

#include "Utils/Logger.hpp"
#include "Gui/GuiManager.hpp"
#include "Gui/GuiRootWindow.hpp"

Player::Player(const QString name)
    : Hittable(name),
      mController(nullptr),
      mCamera(nullptr),
      mStatus(nullptr),
      mMesh(nullptr),
      mIsControllable(false),
      mWalkingSound(nullptr),
      mHUDAmmo(nullptr),
      mHUDHealth(nullptr),
      mHUDClip(nullptr),
      mJumpingSound(nullptr) {}

void Player::OnInitialize() {
    mController = new FPSPlayerComponent(2, "controller");
    mMesh = new dt::MeshComponent("player", "", "player_mesh");
    mStatus = new StatusComponent(100, 100);
    mCamera = new dt::CameraComponent("main_camera");
    mWalkingSound = new dt::SoundComponent("walk.wav", "player_walking_sound");
    mJumpingSound = new dt::SoundComponent("jump.wav", "player_jump_sound");

    const Weapon* weapon = mController->GetWeaponInUse();

    //this->addComponent(mMesh);
    this->addComponent(mStatus);
    this->addComponent(mCamera)->lookAt(Ogre::Vector3(0, 0, -10));
    this->addComponent(mController);
    this->addComponent(mWalkingSound);
    this->addComponent(mJumpingSound);

    dt::GuiRootWindow& win = dt::GuiManager::get()->getRootWindow();
    mHUDAmmo = win.addChildWidget(new dt::GuiButton("HUD_ammo"));
    mHUDHealth = win.addChildWidget(new dt::GuiButton("HUD_health"));
    mHUDClip = win.addChildWidget(new dt::GuiButton("HUD_clip"));
    auto screen_rect = win.getMyGUIWidget()->getAbsoluteRect();

    mHUDHealth->setSize(100, 30);
    mHUDAmmo->setSize(100, 30);
    mHUDClip->setSize(100, 30);

    mHUDHealth->setPosition(10, screen_rect.height() - 50);
    mHUDAmmo->setPosition(screen_rect.width() - 110, screen_rect.height() - 90);
    mHUDClip->setPosition(screen_rect.width() - 110, screen_rect.height() - 50);

    dt::GuiManager::get()->setMouseCursorVisible(false);

    if(mIsControllable)
        mController->enable();
    else
        mController->disable();

    mWalkingSound->getSound().setLoop(true);
    mJumpingSound->setVolume(20);

    _RefreshHealth(0, 100);
    if(weapon != nullptr) {
        _OnWeaponChanged(weapon);
    }

    if(!QObject::connect(mController, SIGNAL(sWeaponChanged(const Weapon*)), 
                         this,        SLOT(_OnWeaponChanged(const Weapon*)), Qt::DirectConnection)) {
            dt::Logger::get().debug(QString("Failed to connect the controller's sWeaponChanged") +
                QString("signal with the player's _OnWeaponChanged"));
    }

    if(!QObject::connect(mController, SIGNAL(sMove()),
                         this,        SLOT(_OnWalk()), Qt::DirectConnection)) {
        dt::Logger::get().debug(QString("Failed to connect the controller's sMove with the player's _OnWalk"));
    }

    if(!QObject::connect(mController, SIGNAL(sStop()),
                         this,        SLOT(_OnStop()), Qt::DirectConnection)) {
        dt::Logger::get().debug(QString("Failed to connect the controller's sStop with the player's _OnStop()"));
    }

    if(!QObject::connect(mController, SIGNAL(sJump()),
                         this,        SLOT(_OnJump()), Qt::DirectConnection)) {
        dt::Logger::get().debug(QString("Failed to connect the controller's sStop with the player's _OnStop()"));
    }
}

void Player::_RefreshAmmo(uint16_t current_ammo) {
    mHUDAmmo->setCaption(QString("Ammo: ") + dt::Utils::toString(current_ammo));
}

void Player::_RefreshClip(uint16_t current_clip) {
    mHUDClip->setCaption(QString("Clip: ") + dt::Utils::toString(current_clip));
}

void Player::_RefreshHealth(uint16_t previous_health, uint16_t current_health) {
    mHUDHealth->setCaption(QString("Health: ") + dt::Utils::toString(current_health));
}

void Player::_OnWeaponChanged(const Weapon* current_weapon) {
    if(!QObject::disconnect(this, SLOT(_RefreshAmmo(uint16_t))))
        dt::Logger::get().debug("Failed to disconnect the player's _RefreshAmmo slot!");
    if(!QObject::disconnect(this, SLOT(_RefreshClip(uint16_t))))
        dt::Logger::get().debug("Failed to disconnect the player's _RefreshClip slot!");

    if(current_weapon == nullptr) {
        _RefreshClip(0);
        _RefreshAmmo(0);
    }
    else {
        if(!QObject::connect(current_weapon, SIGNAL(sAmmoChanged(uint16_t)),
                             this,           SLOT(_RefreshAmmo(uint16_t))))
            dt::Logger::get().debug("Failed to connect the new weapon's sAmmoChanged signal!");
        if(!QObject::connect(current_weapon, SIGNAL(sClipChanged(uint16_t)),
                             this,           SLOT(_RefreshClip(uint16_t))))
            dt::Logger::get().debug("Failed to connect the new weapon's sClipChanged signal!");
       
        _RefreshClip(current_weapon->GetCurrentClip());
        _RefreshAmmo(current_weapon->GetCurrentAmmo());
    }
}

FPSPlayerComponent* Player::GetController() const {
    return mController;
}

dt::CameraComponent* Player::GetCamera() const {
    return mCamera;
}

StatusComponent* Player::GetStatus() const {
    return mStatus;
}

dt::MeshComponent* Player::GetMesh() const {
    return mMesh;
}

void Player::OnHit(int32_t damage) {
    mStatus->SetHealth(mStatus->GetHealth() - damage);
}

void Player::SetControllable(bool is_controllable) {
    if(mIsControllable != is_controllable) {
        mIsControllable = is_controllable;
        if(mIsControllable)
            mController->enable();
        else
            mController->disable();
    }
}

bool Player::IsControllable() const {
    return mIsControllable;
}

void Player::OnEnable() {
    if(!mIsControllable)
        mController->disable();
}

void Player::_OnWalk() {
    mWalkingSound->playSound();
}

void Player::_OnStop() {
    mWalkingSound->stopSound();
}

void Player::_OnJump() {
    mJumpingSound->playSound();
}
