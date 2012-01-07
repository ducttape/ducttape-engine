#include "Player.hpp"

Player::Player(FPSPlayerComponent* controller, dt::CameraComponent* camera, StatusComponent* status,
    dt::MeshComponent* mesh, const QString& name)
    : Node(name),
      mController(controller),
      mCamera(camera),
      mStatus(status),
      mMesh(mesh) {}

void Player::OnInitialize() {
    this->AddComponent(mMesh);
    this->AddComponent(mController);
    this->AddComponent(mStatus);
    this->AddComponent(mCamera)->LookAt(Ogre::Vector3(0, 0, -10));
}