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

void Main::onInitialize() {
    dt::ResourceManager::get()->addDataPath(QDir("./fps/data"));
    dt::ResourceManager::get()->addResourceLocation("gui","FileSystem", true);
    dt::ResourceManager::get()->addResourceLocation("","FileSystem");
    dt::ResourceManager::get()->addResourceLocation("crate", "FileSystem");
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

    dt::Scene* scene = addScene(new dt::Scene("fpsscene"));
    OgreProcedural::Root::getInstance()->sceneManager = scene->getSceneManager();

    OgreProcedural::CapsuleGenerator().setHeight(1.77f).setRadius(0.44f).realizeMesh("player");

    Player* player_node = (Player*)scene->addChildNode(new Player("playernode"));
    player_node->setPosition(Ogre::Vector3(0, 1.75, 5));
    player_node->setControllable(true);

    OgreProcedural::SphereGenerator().setRadius(0.3f).setUTile(.5f).realizeMesh("Bullet");
    dt::CollisionComponent* interactor = new dt::CollisionComponent("Bullet", "interactor");
    interactor->setOffset(1.0f);
    interactor->setRange(20.0f);
    Weapon* weapon = (Weapon*)scene->addChildNode(new Weapon("test_gun", interactor, 20, 5, 60,
        2.0f, 0, "fire.wav", "reload_start.wav", "reload_done.wav", "multigun.mesh"));
    weapon->enablePhysicsBody(false);
    weapon->setPosition(5, 2, 5);
    weapon->enablePhysicsBody(true);

    dt::Node* light_node = scene->addChildNode(new dt::Node("lightnode"));
    light_node->setPosition(Ogre::Vector3(-2000, 2000, 1000));
    light_node->addComponent(new dt::LightComponent("light"));

    OgreProcedural::PlaneGenerator().setSizeX(100.0f).setSizeY(100.0f).setUTile(10.0).setVTile(10.0).realizeMesh("Plane");
    dt::Node* plane_node = scene->addChildNode(new dt::Node("planenode"));
    plane_node->addComponent(new dt::MeshComponent("Plane", "PrimitivesTest/Pebbles", "plane-mesh"));
    plane_node->addComponent(new dt::PhysicsBodyComponent("plane-mesh", "plane-body",
        dt::PhysicsBodyComponent::CONVEX, 0.0f));

    dt::Node* test_object = scene->addChildNode(new dt::Node("testobject"));
    test_object->setPosition(Ogre::Vector3(0, 1, -5));
    test_object->addComponent(new dt::MeshComponent("Crate01.mesh", "", "test-mesh"))->setCastShadows(true);
    test_object->addComponent(new dt::PhysicsBodyComponent("test-mesh", "ball-body",
        dt::PhysicsBodyComponent::BOX))->setMass(1.0f);
}

void Main::updateStateFrame(double simulation_frame_time) {
}
