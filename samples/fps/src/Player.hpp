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
    Player(const QString name = "player");

    void onInitialize();

    void setControllable(bool is_controllable);

    bool isControllable() const;

    void onEnable();

    FPSPlayerComponent* getController() const;

    dt::CameraComponent* getCamera() const;

    StatusComponent* getStatus() const;

    dt::MeshComponent* getMesh() const;

    void onHit(int32_t damage);

private slots:
    void _refreshHealth(uint16_t previous_health, uint16_t current_health);

    void _refreshAmmo(uint16_t current_ammo);

    void _refreshClip(uint16_t current_clip);

    void _onWeaponChanged(const Weapon* current_weapon);

    void _onWalk();

    void _onStop();

    void _onJump();

private:
    FPSPlayerComponent* mController;
    dt::CameraComponent* mCamera;
    StatusComponent* mStatus;
    dt::MeshComponent* mMesh;
    bool mIsControllable;
    dt::SoundComponent* mWalkingSound;
    std::shared_ptr<dt::GuiButton> mHUDAmmo;
    std::shared_ptr<dt::GuiButton> mHUDHealth;
    std::shared_ptr<dt::GuiButton> mHUDClip;
    dt::SoundComponent* mJumpingSound;   
};

#endif
