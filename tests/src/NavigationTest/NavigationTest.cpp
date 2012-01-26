
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

//     dt::Scene* testscene = GetScene("testscene");
// /*    dt::PhysicsBodyComponent* sphere1 = */testscene->FindChildNode("spherenode")->FindComponent<dt::PhysicsBodyComponent>("sphere-body");
//    /* dt::PhysicsBodyComponent* sphere2 = */testscene->FindChildNode("spherenode2")->FindComponent<dt::PhysicsBodyComponent>("sphere-body2");

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

    mNavManager = new dt::NavigationManager(scene->GetSceneManager());
    
    dt::Node* camnode = scene->AddChildNode(new dt::Node("cam-node"));
    camnode->SetPosition(Ogre::Vector3(2, 30, 2));
    camnode->AddComponent(new dt::CameraComponent("cam"))->LookAt(Ogre::Vector3(0, 0, 0));;

    dt::Node* cratenode = scene->AddChildNode(new dt::Node("box-node"));
    cratenode->SetPosition(Ogre::Vector3(-2, 0, 1.3));
    cratenode->AddComponent(new dt::MeshComponent("Box", "PrimitivesTest/Pebbles", "box-mesh"));

    dt::Node* spherenode2 = scene->AddChildNode(new dt::Node("spherenode"));
    spherenode2->SetPosition(Ogre::Vector3(2, 0, -1.3));
    spherenode2->AddComponent(new dt::MeshComponent("Sphere", "PrimitivesTest/RedBrick", "sphere-mesh"));
;

    dt::Node* planenode = scene->AddChildNode(new dt::Node("plane-node"));
    planenode->SetPosition(Ogre::Vector3(0, 0, 0));
    Ogre::Quaternion q;
    planenode->AddComponent(new dt::MeshComponent("Plane", "PrimitivesTest/Pebbles", "plane-mesh"));

    //Add mesh that must be analyzed by the navigation manager.
    mNavManager->AddMesh(*plane_mesh.get(), planenode->GetPosition(), 
            planenode->GetRotation(), planenode->GetScale());
    mNavManager->AddMesh(*box_mesh.get(), cratenode->GetPosition(), 
            cratenode->GetRotation(), cratenode->GetScale());
    mNavManager->AddMesh(*sphere_mesh.get(), spherenode2->GetPosition(), 
                    spherenode2->GetRotation(), spherenode2->GetScale());

    dt::Node* lightnode1 = scene->AddChildNode(new dt::Node("lightnode1"));
    lightnode1->AddComponent(new dt::LightComponent("light1"));
    lightnode1->SetPosition(Ogre::Vector3(0, 25, 0));
    
    dt::Node* meshnode = scene->AddChildNode(new dt::Node("meshnode"));
    meshnode->SetPosition(-8, 0, 0);
    meshnode->SetScale(0.3);
    dt::MeshComponent* mesh = new dt::MeshComponent("Sinbad.mesh", "", "sinbad");
    meshnode->AddComponent(mesh);
    
    // Config the navigation manager.
    rcConfig config = mNavManager->CreateDefaultConfig();
    config.walkableRadius = mesh->GetOgreEntity()->getBoundingRadius() * 30;
    mNavManager->SetConfig(config);
    mNavManager->BuildMap();

    // Try path navigation.
    dt::FollowPathComponent* path = meshnode->AddComponent(new dt::FollowPathComponent(dt::FollowPathComponent::LOOP, "path"));
    path->SetFollowRotation(false);
    path->SetSmoothAcceleration(true);
    path->SetSmoothCorners(false);
    mNavManager->AddPathToComponent(meshnode->GetPosition(), Ogre::Vector3(8, 0, 0), *path);
    path->SetDuration(1.5);
}

}
