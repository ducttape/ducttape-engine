#pragma once

#include <Config.hpp>

#include <Scene/Node.hpp>
#include <Logic/InteractionComponent.hpp>
#include <Graphics/MeshComponent.hpp>
#include <Physics/PhysicsBodyComponent.hpp>
#include <Utils/Timer.hpp>
#include <Audio/SoundComponent.hpp>

#include <QString>

class Weapon : public dt::Node {
    Q_OBJECT
public:
    Weapon(const QString& name, dt::InteractionComponent* interactor, int power, 
        unsigned max_clip, unsigned ammo_per_clip, float reload_time, unsigned type,
        const QString& sound_handle, const QString& mesh_handle, const QString& material_handle = "");

    void OnInitialize();

    void OnDeinitialize();

    int GetPower() const;

    void SetPower(int power);
    
    void Fire();

    void Reload();

    void SetCurrentAmmo(unsigned current_ammo);

    void SetCurrentClip(unsigned current_clip);

    void SetMaxClip(unsigned max_clip);

    void SetAmmoPerClip(unsigned ammo_per_clip);

    unsigned GetCurrentAmmo() const;
    
    unsigned GetCurrentClip() const;

    unsigned GetMaxClip() const;

    unsigned GetAmmoPerClip() const;

    float GetReloadTime() const;

    void SetReloadTime(float reload_time);

    const dt::InteractionComponent* GetInteractor() const;

    unsigned GetType() const;

    void SetType(unsigned type);

    void EnablePhysicsBody(bool is_enabled);

    bool IsPhysicsBodyEnabled() const;

    void OnEnable();

signals:
    void sAmmoChanged(unsigned current_ammo);

    void sClipChanged(unsigned current_clip);

private slots:
    void _OnHit(dt::PhysicsBodyComponent* hit);

    void _OnReloadCompleted();

private:
    dt::InteractionComponent* mInteractor;
    int mPower;
    unsigned mCurrentAmmo;
    unsigned mCurrentClip;
    unsigned mMaxClip;
    unsigned mAmmoPerClip;
    float mReloadTime;
    dt::Timer* mReloadTimer;
    unsigned mType;
    QString mMeshHandle;
    dt::PhysicsBodyComponent* mPhysicsBody;
    bool mIsPhysicsBodyEnabled;
    QString mMaterialHandle;
    QString mSoundHandle;
    dt::SoundComponent* mSound;
};