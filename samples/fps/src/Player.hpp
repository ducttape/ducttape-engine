#ifndef DUCTTAPE_SAMPLE_FPS_PLAYER
#define DUCTTAPE_SAMPLE_FPS_PLAYER

#include "Config.hpp"

#include "FPSPlayerComponent.hpp"
#include "StatusComponent.hpp"
#include "Hittable.hpp"

#include "Graphics/MeshComponent.hpp"
#include "Graphics/TextComponent.hpp"
#include "Graphics/CameraComponent.hpp"
#include "Gui/GuiButton.hpp"

class Player : public Hittable {
    Q_OBJECT
public:
    Player(const QString& name = "player");

    void OnInitialize();

    void SetControllable(bool is_controllable);

    bool IsControllable() const;

    void OnEnable();

    FPSPlayerComponent* GetController() const;

    dt::CameraComponent* GetCamera() const;

    StatusComponent* GetStatus() const;

    dt::MeshComponent* GetMesh() const;

    void OnHit(int32_t damage);

private slots:
    void _RefreshHealth(uint16_t previous_health, uint16_t current_health);

    void _RefreshAmmo(uint16_t current_ammo);

    void _RefreshClip(uint16_t current_clip);

    void _OnWeaponChanged(const Weapon* current_weapon);

    void _OnWalk();

    void _OnStop();

    void _OnJump();

private:
    FPSPlayerComponent* mController;
    dt::CameraComponent* mCamera;
    StatusComponent* mStatus;
    dt::MeshComponent* mMesh;
    bool mIsControllable;
    dt::SoundComponent* mWalkingSound;
    dt::GuiButton* mHUDAmmo;
    dt::GuiButton* mHUDHealth;
    dt::GuiButton* mHUDClip;
    dt::SoundComponent* mJumpingSound;
};

#endif
