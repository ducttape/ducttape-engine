#pragma once

#include <Config.hpp>

#include <Scene/Node.hpp>
#include <Logic/InteractionComponent.hpp>
#include <Graphics/MeshComponent.hpp>
#include <Physics/PhysicsBodyComponent.hpp>
#include <Utils/Timer.hpp>
#include <Audio/SoundComponent.hpp>

#include <QString>

#include <cstdint>

class Weapon : public dt::Node {
    Q_OBJECT
public:
    Weapon(const QString& name, dt::InteractionComponent* interactor, int32_t power, 
        uint16_t max_clip, uint16_t ammo_per_clip, float reload_time, uint16_t type,
        const QString& sound_handle, const QString& reloading_begin_sound_handle, 
        const QString& reloading_done_sound_handle, const QString& mesh_handle, const QString& material_handle = "");

    void OnInitialize();

    void OnDeinitialize();

    int32_t GetPower() const;

    void SetPower(int32_t power);
    
    void Fire();

    void Reload();

    void SetCurrentAmmo(uint16_t current_ammo);

    void SetCurrentClip(uint16_t current_clip);

    void SetMaxClip(uint16_t max_clip);

    void SetAmmoPerClip(uint16_t ammo_per_clip);

    uint16_t GetCurrentAmmo() const;
    
    uint16_t GetCurrentClip() const;

    uint16_t GetMaxClip() const;

    uint16_t GetAmmoPerClip() const;

    float GetReloadTime() const;

    void SetReloadTime(float reload_time);

    const dt::InteractionComponent* GetInteractor() const;

    uint16_t GetType() const;

    void SetType(uint16_t type);

    void EnablePhysicsBody(bool is_enabled);

    bool IsPhysicsBodyEnabled() const;

    void OnEnable();

signals:
    void sAmmoChanged(uint16_t current_ammo);

    void sClipChanged(uint16_t current_clip);

private slots:
    void _OnHit(dt::PhysicsBodyComponent* hit);

    void _OnReloadCompleted();

private:
    dt::InteractionComponent* mInteractor;
    int32_t mPower;
    uint16_t mCurrentAmmo;
    uint16_t mCurrentClip;
    uint16_t mMaxClip;
    uint16_t mAmmoPerClip;
    float mReloadTime;
    dt::Timer* mReloadTimer;
    uint16_t mType;
    QString mMeshHandle;
    dt::PhysicsBodyComponent* mPhysicsBody;
    bool mIsPhysicsBodyEnabled;
    QString mMaterialHandle;
    QString mFiringSoundHandle;
    QString mReloadingBeginSoundHandle;
    QString mReloadingDoneSoundHandle;
    dt::SoundComponent* mFiringSound;
    dt::SoundComponent* mReloadingBeginSound;
    dt::SoundComponent* mReloadingDoneSound;
};