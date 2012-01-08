#ifndef PLAYER_H
#define PLAYER_H
#pragma once

#include "Config.hpp"

#include "FPSPlayerComponent.hpp"
#include "StatusComponent.hpp"
#include "Hittable.hpp"

#include "Graphics/MeshComponent.hpp"
#include "Graphics/TextComponent.hpp"
#include "Graphics/CameraComponent.hpp"

class Player : public Hittable {
    Q_OBJECT
public:
    Player(const QString& name = "player");

    void OnInitialize();

private slots:
    void _RefreshHealth(unsigned previous_health, unsigned current_health);

    void _RefreshAmmo(unsigned current_ammo);

    void _RefreshClip(unsigned current_clip);

    void _OnWeaponChanged(const Weapon* current_weapon);

    const FPSPlayerComponent* GetController() const;

    const dt::CameraComponent* GetCamera() const;

    const StatusComponent* GetStatus() const;

    const dt::MeshComponent* GetMesh() const;

    void OnHit(int damage);

private:
    FPSPlayerComponent* mController;
    dt::CameraComponent* mCamera;
    StatusComponent* mStatus;
    dt::MeshComponent* mMesh;
};

#endif