#include "FPS.hpp"
#include "FastWeaponComponent.hpp"
#include "SlowWeaponComponent.hpp"
#include "FPSPlayerComponent.hpp"

#include <Graphics/CameraComponent.hpp>
#include <Graphics/LightComponent.hpp>
#include <Graphics/MeshComponent.hpp>
#include <Logic/SimplePlayerComponent.hpp>
#include <Physics/PhysicsBodyComponent.hpp>
#include <Logic/AdvancePlayerComponent.hpp>
#include <Physics/PhysicsManager.hpp>

#include <OgreProcedural.h>

void Main::OnInitialize() {
    dt::Scene* scene = AddScene(new dt::Scene("fpsscene"));
    OgreProcedural::Root::getInstance()->sceneManager = scene->GetSceneManager();

    OgreProcedural::SphereGenerator().setRadius(1.f).setUTile(.5f).realizeMesh("Sphere");

    dt::Node* player_node = scene->AddChildNode(new dt::Node("playernode"));
    player_node->SetPosition(Ogre::Vector3(0, 10, 5));

    player_node->AddComponent(new dt::CameraComponent("camera"))->LookAt(Ogre::Vector3(0, 0, -10));

    player_node->AddComponent(new FPSPlayerComponent("controller"));

    dt::Node* light_node = scene->AddChildNode(new dt::Node("lightnode"));
    light_node->SetPosition(Ogre::Vector3(-2000, 2000, 1000));
    light_node->AddComponent(new dt::LightComponent("light"));

    OgreProcedural::PlaneGenerator().setSizeX(100.0f).setSizeY(100.0f).setUTile(10.0).setVTile(10.0).realizeMesh("Plane");
    dt::Node* plane_node = scene->AddChildNode(new dt::Node("planenode"));
    plane_node->AddComponent(new dt::MeshComponent("Plane", "PrimitivesTest/Pebbles", "plane-mesh"));
    plane_node->AddComponent(new dt::PhysicsBodyComponent("plane-mesh", "plane-body"))->SetMass(0.0f);

    dt::Node* test_object = scene->AddChildNode(new dt::Node("testobject"));
    test_object->SetPosition(Ogre::Vector3(0, 1, -5));
    test_object->AddComponent(new dt::MeshComponent("Sphere", "", "test-mesh"));

    dt::PhysicsBodyComponent* ball = test_object->AddComponent(new dt::PhysicsBodyComponent("test-mesh", "ball-body", dt::PhysicsBodyComponent::SPHERE));
    ball->SetMass(1.0f);

    scene->GetPhysicsWorld()->SetShowDebug(true);
}

void Main::UpdateStateFrame(double simulation_frame_time) {
}