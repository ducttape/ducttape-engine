#ifndef DUCTTAPE_SAMPLE_FPS_WEAPON
#define DUCTTAPE_SAMPLE_FPS_WEAPON

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
    Weapon(const QString name, dt::InteractionComponent* interactor, int32_t power,
        uint16_t max_clip, uint16_t ammo_per_clip, float reload_time, uint16_t type,
        const QString sound_handle, const QString reloading_begin_sound_handle,
        const QString reloading_done_sound_handle, const QString mesh_handle, const QString material_handle = "");

    void onInitialize();

    void onDeinitialize();

    int32_t getPower() const;

    void setPower(int32_t power);
    
    void fire();

    void reload();

    void setCurrentAmmo(uint16_t current_ammo);

    void setCurrentClip(uint16_t current_clip);

    void setMaxClip(uint16_t max_clip);

    void setAmmoPerClip(uint16_t ammo_per_clip);

    uint16_t getCurrentAmmo() const;
    
    uint16_t getCurrentClip() const;

    uint16_t getMaxClip() const;

    uint16_t getAmmoPerClip() const;

    float getReloadTime() const;

    void setReloadTime(float reload_time);

    const dt::InteractionComponent* getInteractor() const;

    uint16_t getType() const;

    void setType(uint16_t type);

    void enablePhysicsBody(bool is_enabled);

    bool isPhysicsBodyEnabled() const;

    void onEnable();

signals:
    void sAmmoChanged(uint16_t current_ammo);

    void sClipChanged(uint16_t current_clip);

private slots:
    void _onHit(dt::PhysicsBodyComponent* hit);

    void _onReloadCompleted();

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

#endif
