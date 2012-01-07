#ifndef PLAYER_H
#define PLAYER_H
#pragma once

#include "Config.hpp"

#include "FPSPlayerComponent.hpp"
#include "StatusComponent.hpp"

#include "Scene/Node.hpp"
#include "Graphics/MeshComponent.hpp"
#include "Graphics/TextComponent.hpp"
#include "Graphics/CameraComponent.hpp"

class Player : public dt::Node {
    Q_OBJECT
public:
    Player(FPSPlayerComponent* controller, dt::CameraComponent* camera,
        StatusComponent* status, dt::MeshComponent* mesh, const QString& name = "player");

    void OnInitialize();

private:
    FPSPlayerComponent* mController;
    dt::CameraComponent* mCamera;
    StatusComponent* mStatus;
    dt::MeshComponent* mMesh;
};

#endif