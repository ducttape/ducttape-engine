#include "FPS.hpp"

#include <Graphics/CameraComponent.hpp>
#include <Graphics/LightComponent.hpp>
#include <Graphics/MeshComponent.hpp>
#include <Logic/SimplePlayerComponent.hpp>
#include <Physics/PhysicsBodyComponent.hpp>
#include <Logic/FPSPlayerComponent.hpp>
#include <FastWeaponComponent.hpp>

#include <OgreProcedural.h>

void Main::OnInitialize() {
    dt::Scene* scene = AddScene(new dt::Scene("fpsscene"));
    OgreProcedural::Root::getInstance()->sceneManager = scene->GetSceneManager();

    OgreProcedural::SphereGenerator().setRadius(1.f).setUTile(.5f).realizeMesh("Sphere");

    dt::Node* player_node = scene->AddChildNode(new dt::Node("playernode"));
    player_node->SetPosition(Ogre::Vector3(0, 10, 0));

    player_node->AddComponent(new dt::CameraComponent("camera"))->LookAt(Ogre::Vector3(0, 0, -10));

    FastWeaponComponent* fast_weapon = player_node->AddComponent(new FastWeaponComponent("gun"));
    fast_weapon->SetRange(10.0f);
    fast_weapon->SetPower(70.0f);

    player_node->AddComponent(new dt::FPSPlayerComponent("controller"))->SetInteractionComponentName("gun");

    dt::Node* light_node = scene->AddChildNode(new dt::Node("lightnode"));
    light_node->SetPosition(Ogre::Vector3(-2000, 2000, 1000));
    light_node->AddComponent(new dt::LightComponent("light"));

    OgreProcedural::PlaneGenerator().setSizeX(200.0f).setSizeY(200.0f).realizeMesh("Plane");
    dt::Node* plane_node = scene->AddChildNode(new dt::Node("planenode"));
    plane_node->AddComponent(new dt::MeshComponent("Plane", "", "plane-mesh"));
    plane_node->AddComponent(new dt::PhysicsBodyComponent("plane-mesh", "plane-body"))->SetMass(0.0);

    dt::Node* test_object = scene->AddChildNode(new dt::Node("testobject"));
    test_object->SetPosition(Ogre::Vector3(5, 10, 0));
    test_object->AddComponent(new dt::MeshComponent("Sphere", "", "test-mesh"));

    dt::PhysicsBodyComponent* ball = test_object->AddComponent(new dt::PhysicsBodyComponent("test-mesh", "ball-body"));
    ball->SetMass(10.0f);
    ball->DisableSleep(true);
}

void Main::UpdateStateFrame(double simulation_frame_time) {
}