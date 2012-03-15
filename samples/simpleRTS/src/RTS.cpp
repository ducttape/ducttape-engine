#include "RTS.hpp"

#include "Crowd.hpp"
#include "Being.hpp"
#include "Soldier.hpp"
#include "Building.hpp"

#include <Core/Root.hpp>
#include <Graphics/LightComponent.hpp>
#include <Graphics/MeshComponent.hpp>
#include <Graphics/CameraComponent.hpp>
#include <Graphics/TerrainManager.hpp>
#include <Physics/PhysicsBodyComponent.hpp>
#include "Logic/FollowPathComponent.hpp"
#include <Logic/SimplePlayerComponent.hpp>
#include <Scene/Scene.hpp>
#include <Utils/Utils.hpp>
#include <Input/InputManager.hpp>
#include <Core/ResourceManager.hpp>
#include <Utils/Logger.hpp>

#include <OgreFontManager.h>
#include <OgreProcedural.h>
#include "Player.hpp"
#include "PlayerCPU.hpp"
#include "Mine.hpp"

float MAP_WIDTH = 300;
float MAP_HEIGHT = 300;

void Main::ResetGame() {}

void Main::OnInitialize() {
    dt::Scene::SceneSP scene(new dt::Scene("RTScene"));
    AddScene(scene);
    OgreProcedural::Root::getInstance()->sceneManager = scene->GetSceneManager();

    dt::ResourceManager::Get()->AddResourceLocation("","FileSystem", true);
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
    
    scene->GetPhysicsWorld()->SetEnabled(true);

    std::shared_ptr<dt::Node> cam = scene->AddChildNode(new dt::Node("cam"));
    cam->SetPosition(Ogre::Vector3(200, 800, 900));
    cam->AddComponent(new dt::CameraComponent("cam_camera"))->LookAt(Ogre::Vector3(0, 0, 0));
    /*dt::SimplePlayerComponent* */cam_player = new dt::SimplePlayerComponent("player");
    cam->AddComponent(cam_player);
    cam_player->SetWASDEnabled(true);
    cam_player->SetMoveSpeed(500);
    cam_player->SetConstant(true);

    std::shared_ptr<dt::Node> zenit_sun = scene->AddChildNode(new dt::Node("zenit_sun"));
    zenit_sun->SetPosition(Ogre::Vector3(0, 50, 0));
    dt::LightComponent* zenit_sun_light = new dt::LightComponent("zenit_sun_light");
    zenit_sun->AddComponent(zenit_sun_light);

    mGameNode = scene->AddChildNode(new dt::Node("game"));

//     std::vector<QString> terrain_files;
//     terrain_files.push_back("terrain.png");
//     std::vector<QString> texture_files;
//     texture_files.push_back("pebbles_diffusespecular.jpg");
//     texture_files.push_back("pebbles_normalheight.jpg");
// 
//     dt::TerrainManager* terrain = dt::TerrainManager::Get();
//     terrain->SetScene(scene);
//     terrain->SetLight(zenit_sun_light);
//     terrain->AddTextureLayer(texture_files, 100, 0, 0);
//     terrain->SetScale(300.0f);
//     terrain->Import(terrain_files);
    
    // create a plane for fake terrain physics. TODO: implement real terrain Physics
    OgreProcedural::PlaneGenerator().setSizeX(1000.f).setSizeY(1000.f).realizeMesh("Plane");
    std::shared_ptr<dt::Node> fake_terrain = scene->AddChildNode(new dt::Node("planenode"));
    fake_terrain->SetPosition(Ogre::Vector3(0, -6, 0));
    
    fake_terrain->AddComponent(new dt::MeshComponent("Plane", "", "plane-mesh"));
    fake_terrain->AddComponent(new dt::PhysicsBodyComponent("plane-mesh", "plane-body"))->SetMass(0.f);
    
    Being::SetScene(scene);
    
    new PlayerCPU();
    new PlayerCPU();
    
//     new Mine(Ogre::Vector3(200,0,200), 0);

}

void Main::UpdateStateFrame(double simulation_frame_time) {
//     GameMap::UpdateMap(simulation_frame_time);
    Being::UpdateBeings(simulation_frame_time);
    Player::UpdatePlayers(simulation_frame_time);
    
    mTime += simulation_frame_time; 
}
