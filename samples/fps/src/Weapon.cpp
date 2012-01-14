#include "Weapon.hpp"
#include "StatusComponent.hpp"
#include "Hittable.hpp"

#include "Utils/Utils.hpp"

Weapon::Weapon(const QString& name, dt::InteractionComponent* interactor, int power, unsigned max_clip, 
    unsigned ammo_per_clip, float reload_time, unsigned type, const QString& sound_handle, 
    const QString& mesh_handle, const QString& material_handle)
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
      mSoundHandle(sound_handle),
      mSound(nullptr) {}

void Weapon::OnInitialize() {
    this->AddComponent(mInteractor);

    this->AddComponent(new dt::MeshComponent(mMeshHandle, mMaterialHandle, "weapon-mesh"));
    mPhysicsBody = this->AddComponent(new dt::PhysicsBodyComponent("weapon-mesh", "weapon-body",
        dt::PhysicsBodyComponent::BOX));

    mIsPhysicsBodyEnabled = true;

    if(!QObject::connect(mInteractor, SIGNAL(sHit(dt::PhysicsBodyComponent*)), 
        this, SLOT(_OnHit(dt::PhysicsBodyComponent*)))) {
            dt::Logger::Get().Error("Cannot connect the sHit signal with the OnHit slot.");
    }

    if(mSoundHandle != "") {
        mSound = (dt::SoundComponent*)this->AddComponent(new dt::SoundComponent(mSoundHandle, this->GetName() + "_fire_sound"));
        mSound->SetVolume(100.0f);
    }
}

int Weapon::GetPower() const {
    return mPower;
}

void Weapon::SetPower(int power) {
    mPower = power;
}

void Weapon::Fire() {
    if(mCurrentAmmo > 0 && mInteractor->IsReady()) {
        if(mSound != nullptr) {
            mSound->StopSound();
            mSound->PlaySound();
        }

        this->mInteractor->Check();
        SetCurrentAmmo(mCurrentAmmo - 1);
    }
    else {
        this->Reload();
    }
}

void Weapon::Reload() {
    if(mCurrentClip > 0 && mReloadTimer == nullptr && mCurrentAmmo < mAmmoPerClip) {
        mReloadTimer = new dt::Timer("Weapon reloaded", mReloadTime, false);
        if(!QObject::connect(mReloadTimer, SIGNAL(TimerStoped()),
            this, SLOT(_OnReloadCompleted()))) {
                dt::Logger::Get().Error("Cannot connect weapon " + this->GetName() + 
                    "'s _OnReloadCompleted slot with its reload timer's signal");
        }
    }
}

void Weapon::SetCurrentAmmo(unsigned current_ammo) {
    if(current_ammo <= mAmmoPerClip)
        mCurrentAmmo = current_ammo;
    else
        mCurrentAmmo = mAmmoPerClip;

    emit sAmmoChanged(mCurrentAmmo);
}

void Weapon::SetAmmoPerClip(unsigned ammo_per_clip) {
    mAmmoPerClip = ammo_per_clip;

    if(mCurrentAmmo > mAmmoPerClip)
        this->SetCurrentAmmo(mAmmoPerClip);
}

void Weapon::SetCurrentClip(unsigned current_clip) {
    if(current_clip <= mMaxClip)
        mCurrentClip = current_clip;
    else
        mCurrentClip = mMaxClip;

    emit sClipChanged(mCurrentClip);
}

void Weapon::SetMaxClip(unsigned max_clip) {
    mMaxClip = max_clip;

    if(mCurrentClip > mMaxClip)
        this->SetCurrentClip(mMaxClip);
}

unsigned Weapon::GetAmmoPerClip() const {
    return mAmmoPerClip;
}

unsigned Weapon::GetCurrentAmmo() const {
    return mCurrentAmmo;
}

unsigned Weapon::GetCurrentClip() const {
    return mCurrentClip;
}

unsigned Weapon::GetMaxClip() const {
    return mMaxClip;
}

float Weapon::GetReloadTime() const {
    return mReloadTime;
}

void Weapon::SetReloadTime(float reload_time) {
    mReloadTime = reload_time;
}

const dt::InteractionComponent* Weapon::GetInteractor() const {
    return mInteractor;
}

void Weapon::_OnHit(dt::PhysicsBodyComponent* hit) {
    Hittable* obj = dynamic_cast<Hittable*>(hit->GetNode());

    if(obj != nullptr)
        obj->OnHit(this->GetPower());
}

void Weapon::_OnReloadCompleted() {
    if(mCurrentClip > 0 && mCurrentAmmo < mAmmoPerClip) {
        delete mReloadTimer;
        mReloadTimer = nullptr;

        this->SetCurrentClip(mCurrentClip - 1);
        this->SetCurrentAmmo(mAmmoPerClip);
    }
}

unsigned Weapon::GetType() const {
    return mType;
}

void Weapon::SetType(unsigned type) {
    mType = type;
}

void Weapon::EnablePhysicsBody(bool is_enabled) {
    if(is_enabled)
        mPhysicsBody->Enable();
    else
        mPhysicsBody->Disable();

    mIsPhysicsBodyEnabled = is_enabled;
}

bool Weapon::IsPhysicsBodyEnabled() const {
    return mIsPhysicsBodyEnabled;
}

void Weapon::OnEnable() {
    if(!mIsPhysicsBodyEnabled)
        mPhysicsBody->Disable();

    emit sAmmoChanged(mCurrentAmmo);
    emit sClipChanged(mCurrentClip);
}

void Weapon::OnDeinitialize() {
    if(mSound != nullptr) {
        mSound->StopSound();
        QString name = mSound->GetName();
        this->RemoveComponent(mSound->GetName());
    }
    delete mReloadTimer;
}