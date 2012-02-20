#include "FPS.hpp"
#include "FPSPlayerComponent.hpp"
#include "Player.hpp"

#include <Graphics/CameraComponent.hpp>
#include <Graphics/LightComponent.hpp>
#include <Graphics/MeshComponent.hpp>
#include <Logic/CollisionComponent.hpp>
#include <Logic/SimplePlayerComponent.hpp>
#include <Physics/PhysicsBodyComponent.hpp>
#include <Logic/AdvancedPlayerComponent.hpp>
#include <Physics/PhysicsManager.hpp>
#include <Core/ResourceManager.hpp>
#include <Input/InputManager.hpp>

#include <OgreProcedural.h>

void Main::OnInitialize() {
    dt::ResourceManager::Get()->AddDataPath(QDir("./fps/data"));
    dt::ResourceManager::Get()->AddResourceLocation("gui","FileSystem", true);
    dt::ResourceManager::Get()->AddResourceLocation("","FileSystem");
    dt::ResourceManager::Get()->AddResourceLocation("crate", "FileSystem");
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

    dt::Scene* scene = AddScene(new dt::Scene("fpsscene"));
    OgreProcedural::Root::getInstance()->sceneManager = scene->GetSceneManager();

    OgreProcedural::CapsuleGenerator().setHeight(1.77f).setRadius(0.44f).realizeMesh("player");

    Player* player_node = (Player*)scene->AddChildNode(new Player("playernode"));
    player_node->SetPosition(Ogre::Vector3(0, 1.75, 5));
    player_node->SetControllable(true);

    OgreProcedural::SphereGenerator().setRadius(0.3f).setUTile(.5f).realizeMesh("Bullet");
    dt::CollisionComponent* interactor = new dt::CollisionComponent("Bullet", "interactor");
    interactor->SetOffset(1.0f);
    interactor->SetRange(20.0f);
    Weapon* weapon = (Weapon*)scene->AddChildNode(new Weapon("test_gun", interactor, 20, 5, 60, 
        2.0f, 0, "fire.wav", "reload_start.wav", "reload_done.wav", "multigun.mesh"));
    weapon->EnablePhysicsBody(false);
    weapon->SetPosition(5, 2, 5);
    weapon->EnablePhysicsBody(true);

    dt::Node* light_node = scene->AddChildNode(new dt::Node("lightnode"));
    light_node->SetPosition(Ogre::Vector3(-2000, 2000, 1000));
    light_node->AddComponent(new dt::LightComponent("light"));

    OgreProcedural::PlaneGenerator().setSizeX(100.0f).setSizeY(100.0f).setUTile(10.0).setVTile(10.0).realizeMesh("Plane");
    dt::Node* plane_node = scene->AddChildNode(new dt::Node("planenode"));
    plane_node->AddComponent(new dt::MeshComponent("Plane", "PrimitivesTest/Pebbles", "plane-mesh"));
    plane_node->AddComponent(new dt::PhysicsBodyComponent("plane-mesh", "plane-body",
        dt::PhysicsBodyComponent::CONVEX, 0.0f));

    dt::Node* test_object = scene->AddChildNode(new dt::Node("testobject"));
    test_object->SetPosition(Ogre::Vector3(0, 1, -5));
    test_object->AddComponent(new dt::MeshComponent("Crate01.mesh", "", "test-mesh"))->SetCastShadows(true);
    test_object->AddComponent(new dt::PhysicsBodyComponent("test-mesh", "ball-body",
        dt::PhysicsBodyComponent::BOX))->SetMass(1.0f);
}

void Main::UpdateStateFrame(double simulation_frame_time) {
}