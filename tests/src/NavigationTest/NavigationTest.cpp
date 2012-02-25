
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include "NavigationTest/NavigationTest.hpp"

#include <Scene/StateManager.hpp>
#include <Core/ResourceManager.hpp>
#include <Graphics/CameraComponent.hpp>
#include <Logic/FollowPathComponent.hpp>
#include <Logic/NavigationManager.hpp>
#include <Logic/AgentComponent.hpp>

namespace NavigationTest {


bool NavigationTest::Run(int argc, char** argv) {
    dt::Game game;
    game.Run(new Main(), argc, argv);
    return true;
}

QString NavigationTest::GetTestName() {
    return "Navigation";
}

////////////////////////////////////////////////////////////////

Main::Main()
    : mRuntime(0) {}
    
Main::~Main() {
    delete mNavManager;    
    }

void Main::UpdateStateFrame(double simulation_frame_time) {
    mRuntime += simulation_frame_time;
    
    if(mRuntime > 5.0) {
        dt::StateManager::Get()->Pop(1);
    }
}



void Main::OnInitialize() {
    dt::Scene* scene = AddScene(new dt::Scene("testscene"));

    dt::ResourceManager::Get()->AddResourceLocation("","FileSystem");
    dt::ResourceManager::Get()->AddResourceLocation("sinbad.zip","Zip", true);
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

    OgreProcedural::Root::getInstance()->sceneManager = scene->GetSceneManager();

    Ogre::MeshPtr sphere_mesh = OgreProcedural::SphereGenerator().setRadius(1.4f).setUTile(.5f).realizeMesh("Sphere");
    Ogre::MeshPtr box_mesh = OgreProcedural::BoxGenerator().setSizeX(2.0).setSizeY(2.f).setSizeZ(2.f).realizeMesh("Box");
    Ogre::MeshPtr plane_mesh = OgreProcedural::PlaneGenerator().setSizeX(30.f).setSizeY(30.f).realizeMesh("Plane");

    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

    mNavManager = dt::NavigationManager::Get();
    
    dt::Node* cam_node = scene->AddChildNode(new dt::Node("cam_node"));
    cam_node->SetPosition(Ogre::Vector3(22, 30, 22));
    cam_node->AddComponent(new dt::CameraComponent("cam"))->LookAt(Ogre::Vector3(0, 0, 0));;

    dt::Node* box_node = scene->AddChildNode(new dt::Node("box_node"));
    box_node->SetPosition(Ogre::Vector3(-2, 0, 1.3));
    dt::MeshComponent* box_mesh_component = new dt::MeshComponent("Box", "PrimitivesTest/Pebbles", "box_mesh");
    box_node->AddComponent(box_mesh_component);

    dt::Node* sphere_node = scene->AddChildNode(new dt::Node("spherenode"));
    sphere_node->SetPosition(Ogre::Vector3(2, 0, -1.3));
    dt::MeshComponent* sphere_mesh_component = new dt::MeshComponent("Sphere", "PrimitivesTest/RedBrick", "sphere_mesh");
    sphere_node->AddComponent(sphere_mesh_component);
;

    dt::Node* plane_node = scene->AddChildNode(new dt::Node("plane-node"));
    plane_node->SetPosition(Ogre::Vector3(0, 0, 0));
    dt::MeshComponent* plane_mesh_component = new dt::MeshComponent("Plane", "PrimitivesTest/Pebbles", "plane_mesh");
    plane_node->AddComponent(plane_mesh_component);

    //Add mesh that must be analyzed by the navigation manager.
    mNavManager->AddMesh(box_mesh_component);
    mNavManager->AddMesh(sphere_mesh_component);
    mNavManager->AddMesh(plane_mesh_component);

    dt::Node* light_node = scene->AddChildNode(new dt::Node("lightnode1"));
    light_node->AddComponent(new dt::LightComponent("light1"));
    light_node->SetPosition(Ogre::Vector3(0, 25, 0));
    
    dt::Node* simbad_node = scene->AddChildNode(new dt::Node("simbad_node"));
    simbad_node->SetPosition(-8, 0, 0);
    simbad_node->SetScale(0.3);
    dt::MeshComponent* mesh = new dt::MeshComponent("Sinbad.mesh", "", "sinbad");
    simbad_node->AddComponent(mesh);
    
    // Config the navigation manager.
    rcConfig config = mNavManager->CreateDefaultConfig();
    config.walkableRadius = mesh->GetOgreEntity()->getBoundingRadius() * 30;
    mNavManager->SetConfig(config);
    mNavManager->InitDebugDrawer(scene->GetSceneManager());
    mNavManager->BuildMap();

    // Try path navigation.
    dt::FollowPathComponent* path = simbad_node->AddComponent(new dt::FollowPathComponent(dt::FollowPathComponent::LOOP, "path"));
    path->SetFollowRotation(false);
    path->SetSmoothAcceleration(true);
    path->SetSmoothCorners(false);
    mNavManager->AddPathToComponent(simbad_node->GetPosition(), Ogre::Vector3(8, 0, 0), *path);
    path->SetDuration(1.5);
    
    // Create a crowd.
    dt::Crowd* crowd = mNavManager->CreateCrowd();
    dtCrowdAgentParams params = crowd->CreateDefaultConfig();
    // Create 2 crowd agent.
    dt::Node* agentA = scene->AddChildNode(new dt::Node("agentAnode"));
    agentA->SetPosition(-8, 0, 5);
    agentA->SetScale(0.3);
    dt::MeshComponent* agentA_mesh = new dt::MeshComponent("Sinbad.mesh", "", "sinbad-A");
    agentA->AddComponent(agentA_mesh);
    dt::AgentComponent* agentA_comp = crowd->CreateAgentComponent(agentA->GetPosition(), params, "agent-A");
    agentA->AddComponent(agentA_comp);
    agentA_comp->MoveTo(Ogre::Vector3(8, 0, 5));
    dt::Node* agentB = scene->AddChildNode(new dt::Node("agentBnode"));
    agentB->SetPosition(8, 0, 5);
    agentB->SetScale(0.3);
    dt::MeshComponent* agentB_mesh = new dt::MeshComponent("Sinbad.mesh", "", "sinbad-B");
    agentB->AddComponent(agentB_mesh);
    dt::AgentComponent* agentB_comp = crowd->CreateAgentComponent(agentB->GetPosition(), params, "agent-B");
    agentB->AddComponent(agentB_comp);
    agentB_comp->MoveTo(Ogre::Vector3(-8, 0, 5));
    
}

}
