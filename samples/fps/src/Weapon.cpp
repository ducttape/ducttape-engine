#include "Weapon.hpp"
#include "StatusComponent.hpp"
#include "Hittable.hpp"

#include "Utils/Utils.hpp"

Weapon::Weapon(const QString name, dt::InteractionComponent* interactor, int32_t power, uint16_t max_clip,
    uint16_t ammo_per_clip, float reload_time, uint16_t type, const QString firing_sound_handle,
    const QString reloading_begin_sound_handle, const QString reloading_done_sound_handle,
    const QString mesh_handle, const QString material_handle)
    : Node(name),
      mInteractor(interactor),
      mPower(power),
      mCurrentAmmo(ammo_per_clip),
      mCurrentClip(max_clip),
      mMaxClip(max_clip),
      mAmmoPerClip(ammo_per_clip),
      mReloadTime(reload_time), 
      mReloadTimer(nullptr),
      mType(type),
      mMeshHandle(mesh_handle),
      mPhysicsBody(nullptr),
      mIsPhysicsBodyEnabled(false),
      mMaterialHandle(material_handle),
      mFiringSoundHandle(firing_sound_handle),
      mReloadingBeginSoundHandle(reloading_begin_sound_handle),
      mReloadingDoneSoundHandle(reloading_done_sound_handle),
      mFiringSound(nullptr),
      mReloadingBeginSound(nullptr),
      mReloadingDoneSound(nullptr) {}

void Weapon::onInitialize() {
    this->addComponent(mInteractor);

    this->addComponent(new dt::MeshComponent(mMeshHandle, mMaterialHandle, "weapon-mesh"));
    mPhysicsBody = this->addComponent(new dt::PhysicsBodyComponent("weapon-mesh", "weapon-body",
        dt::PhysicsBodyComponent::BOX));

    mIsPhysicsBodyEnabled = true;

    if(!QObject::connect(mInteractor, SIGNAL(sHit(dt::PhysicsBodyComponent*)), 
                         this,        SLOT(_onHit(dt::PhysicsBodyComponent*)))) {
            dt::Logger::get().error("Cannot connect the sHit signal with the OnHit slot.");
    }

    if(mFiringSoundHandle != "") {
        mFiringSound = this->addComponent(new dt::SoundComponent(mFiringSoundHandle,
            this->getName() + "_firing_sound"));
        mFiringSound->setVolume(100.0f);
    }

    if(mReloadingBeginSoundHandle != "") {
        mReloadingBeginSound = this->addComponent(new dt::SoundComponent(mReloadingBeginSoundHandle,
            this->getName() + "_reloading_begin_sound"));
        mReloadingBeginSound->setVolume(100.0f);
    }

    if(mReloadingDoneSoundHandle != "") {
        mReloadingDoneSound = this->addComponent(new dt::SoundComponent(mReloadingDoneSoundHandle,
            this->getName() + "_reloading_done_sound"));
        mReloadingDoneSound->setVolume(100.0f);
    }
}

int32_t Weapon::getPower() const {
    return mPower;
}

void Weapon::setPower(int power) {
    mPower = power;
}

void Weapon::fire() {
    if(mCurrentAmmo > 0) {
        if(mInteractor->isReady()) {
            if(mFiringSound != nullptr) {
                mFiringSound->stopSound();
                mFiringSound->playSound();
            }

            this->mInteractor->check();
            setCurrentAmmo(mCurrentAmmo - 1);
        }
    }
    else {
        this->reload();
    }
}

void Weapon::reload() {
    if(mCurrentClip > 0 && mReloadTimer == nullptr && mCurrentAmmo < mAmmoPerClip) {
        mReloadTimer = new dt::Timer("Weapon reloaded", mReloadTime, false);
        mReloadingBeginSound->playSound();
        if(!QObject::connect(mReloadTimer, SIGNAL(timerStoped()),
                             this,         SLOT(_onReloadCompleted()))) {
                dt::Logger::get().error("Cannot connect weapon " + this->getName() +
                    "'s _OnReloadCompleted slot with its reload timer's signal");
        }
    }
}

void Weapon::setCurrentAmmo(uint16_t current_ammo) {
    if(current_ammo <= mAmmoPerClip)
        mCurrentAmmo = current_ammo;
    else
        mCurrentAmmo = mAmmoPerClip;

    emit sAmmoChanged(mCurrentAmmo);
}

void Weapon::setAmmoPerClip(uint16_t ammo_per_clip) {
    mAmmoPerClip = ammo_per_clip;

    if(mCurrentAmmo > mAmmoPerClip)
        this->setCurrentAmmo(mAmmoPerClip);
}

void Weapon::setCurrentClip(uint16_t current_clip) {
    if(current_clip <= mMaxClip)
        mCurrentClip = current_clip;
    else
        mCurrentClip = mMaxClip;

    emit sClipChanged(mCurrentClip);
}

void Weapon::setMaxClip(uint16_t max_clip) {
    mMaxClip = max_clip;

    if(mCurrentClip > mMaxClip)
        this->setCurrentClip(mMaxClip);
}

uint16_t Weapon::getAmmoPerClip() const {
    return mAmmoPerClip;
}

uint16_t Weapon::getCurrentAmmo() const {
    return mCurrentAmmo;
}

uint16_t Weapon::getCurrentClip() const {
    return mCurrentClip;
}

uint16_t Weapon::getMaxClip() const {
    return mMaxClip;
}

float Weapon::getReloadTime() const {
    return mReloadTime;
}

void Weapon::setReloadTime(float reload_time) {
    mReloadTime = reload_time;
}

const dt::InteractionComponent* Weapon::getInteractor() const {
    return mInteractor;
}

void Weapon::_onHit(dt::PhysicsBodyComponent* hit) {
    Hittable* obj = dynamic_cast<Hittable*>(hit->getNode());

    if(obj != nullptr)
        obj->onHit(this->getPower());
}

void Weapon::_onReloadCompleted() {
    if(mCurrentClip > 0 && mCurrentAmmo < mAmmoPerClip) {
        delete mReloadTimer;
        mReloadTimer = nullptr;

        mReloadingDoneSound->playSound();
        this->setCurrentClip(mCurrentClip - 1);
        this->setCurrentAmmo(mAmmoPerClip);
    }
}

uint16_t Weapon::getType() const {
    return mType;
}

void Weapon::setType(uint16_t type) {
    mType = type;
}

void Weapon::enablePhysicsBody(bool is_enabled) {
    if(is_enabled)
        mPhysicsBody->enable();
    else
        mPhysicsBody->disable();

    mIsPhysicsBodyEnabled = is_enabled;
}

bool Weapon::isPhysicsBodyEnabled() const {
    return mIsPhysicsBodyEnabled;
}

void Weapon::onEnable() {
    if(!mIsPhysicsBodyEnabled)
        mPhysicsBody->disable();

    emit sAmmoChanged(mCurrentAmmo);
    emit sClipChanged(mCurrentClip);
}

void Weapon::onDeinitialize() {
    if(mReloadTimer != nullptr)
        delete mReloadTimer;
}
