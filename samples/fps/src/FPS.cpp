#include "FPS.hpp"

#include <Graphics/CameraComponent.hpp>
#include <Graphics/LightComponent.hpp>
#include <Graphics/MeshComponent.hpp>
#include <Logic/SimplePlayerComponent.hpp>
#include <Physics/PhysicsBodyComponent.hpp>
#include <Logic/FPSPlayerComponent.hpp>

#include <OgreProcedural.h>

void Main::OnInitialize() {
    dt::Scene* scene = AddScene(new dt::Scene("fpsscene"));
    OgreProcedural::Root::getInstance()->sceneManager = scene->GetSceneManager();

    dt::Node* player_node = scene->AddChildNode(new dt::Node("playernode"));
    player_node->SetPosition(Ogre::Vector3(0, 100, 0));

    player_node->AddComponent(new dt::CameraComponent("camera"))->LookAt(Ogre::Vector3(0, 0, -10));

    OgreProcedural::SphereGenerator().setRadius(1.f).setUTile(.5f).realizeMesh("Sphere");

    dt::FPSPlayerComponent* controller = player_node->AddComponent(new dt::FPSPlayerComponent("controller"));

    dt::Node* light_node = scene->AddChildNode(new dt::Node("lightnode"));
    light_node->SetPosition(Ogre::Vector3(-20, 20, 10));
    light_node->AddComponent(new dt::LightComponent("light"));

    OgreProcedural::PlaneGenerator().setSizeX(20.0f).setSizeY(20.0f).realizeMesh("Plane");
    dt::Node* plane_node = scene->AddChildNode(new dt::Node("planenode"));
    plane_node->AddComponent(new dt::MeshComponent("Plane", "", "plane-mesh"));
    plane_node->AddComponent(new dt::PhysicsBodyComponent("plane-mesh", "plane-body"))->SetMass(0.0);
}

void Main::UpdateStateFrame(double simulation_frame_time) {
}