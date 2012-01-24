
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
    dt::ResourceManager::Get()->AddResourceLocation("crate","FileSystem");
    dt::ResourceManager::Get()->AddResourceLocation("sinbad.zip","Zip", true);
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

    OgreProcedural::Root::getInstance()->sceneManager = scene->GetSceneManager();

    Ogre::MeshPtr sphere_mesh = OgreProcedural::SphereGenerator().setRadius(1.f).setUTile(.5f).realizeMesh("Sphere");
    Ogre::MeshPtr box_mesh = OgreProcedural::BoxGenerator().setSizeX(5.0).setSizeY(5.f).setSizeZ(5.f).realizeMesh("Box");
    Ogre::MeshPtr plane_mesh = OgreProcedural::PlaneGenerator().setSizeX(30.f).setSizeY(30.f).realizeMesh("Plane");

    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

        mNavManager = new dt::NavigationManager(scene->GetSceneManager());
    
    dt::Node* camnode = scene->AddChildNode(new dt::Node("camnode"));
    camnode->SetPosition(Ogre::Vector3(0, 35, 0));
//     camnode->SetPosition(Ogre::Vector3(0, 110, 0));
    camnode->AddComponent(new dt::CameraComponent("cam"))->LookAt(Ogre::Vector3(0, 0, 0));;

    dt::Node* cratenode = scene->AddChildNode(new dt::Node("cratenode"));
    cratenode->SetPosition(Ogre::Vector3(-2, 0, 0));
//     cratenode->AddComponent(new dt::MeshComponent("Crate01.mesh", "", "crate-mesh"));
//     Ogre::MeshPtr crate_mesh = cratenode->FindComponent<dt::MeshComponent>("crate-mesh")->GetOgreEntity()->getMesh();
//     mNavManager->AddMesh(*crate_mesh.get(), cratenode->GetPosition(), 
//                 cratenode->GetRotation(), cratenode->GetScale());
    cratenode->AddComponent(new dt::MeshComponent("Box", "PrimitivesTest/Pebbles", "box-mesh"));
//     mNavManager->AddMesh(*box_mesh.get(), cratenode->GetPosition(), 
//                 cratenode->GetRotation(), cratenode->GetScale());

//     dt::Node* spherenode2 = scene->AddChildNode(new dt::Node("spherenode"));
//     spherenode2->SetPosition(Ogre::Vector3(2, 0, 0));
//     spherenode2->AddComponent(new dt::MeshComponent("Sphere", "PrimitivesTest/RedBrick", "sphere-mesh"));
//     mNavManager->AddMesh(*sphere_mesh.get(), spherenode2->GetPosition(), 
//                         spherenode2->GetRotation(), spherenode2->GetScale());

    dt::Node* planenode = scene->AddChildNode(new dt::Node("planenode"));
    planenode->SetPosition(Ogre::Vector3(0, 0, 0));
    Ogre::Quaternion q;
    planenode->AddComponent(new dt::MeshComponent("Plane", "PrimitivesTest/Pebbles", "plane-mesh"));
    
        mNavManager->AddMesh(*box_mesh.get(), cratenode->GetPosition(), 
                cratenode->GetRotation(), cratenode->GetScale());
    mNavManager->AddMesh(*plane_mesh.get(), planenode->GetPosition(), 
                    planenode->GetRotation(), planenode->GetScale());
//     mNavManager->AddMesh(*box_mesh.get(), cratenode->GetPosition(), 
//                 cratenode->GetRotation(), cratenode->GetScale());
//     
//     dt::ResourceManager::Get()->AddResourceLocation("main","FileSystem");
//     dt::MeshComponent* main_mesh = new dt::MeshComponent("main.mesh", "", "main");
//     planenode->AddComponent(main_mesh);
//     mNavManager->AddMesh(*main_mesh->GetOgreEntity()->getMesh().get(), planenode->GetPosition(), 
//                 planenode->GetRotation(), planenode->GetScale());

    dt::Node* lightnode1 = scene->AddChildNode(new dt::Node("lightnode1"));
    lightnode1->AddComponent(new dt::LightComponent("light1"));
    lightnode1->SetPosition(Ogre::Vector3(15, 5, 15));
    
    mNavManager->BuildMap();
    
    // Sample #1: LOOP, SmoothAcceleration, Sharp corners, No rotation
    dt::Node* meshnode = scene->AddChildNode(new dt::Node("meshnode"));
    meshnode->SetPosition(-2, 0, -8);
    meshnode->SetScale(0.3);
    dt::FollowPathComponent* path = meshnode->AddComponent(new dt::FollowPathComponent(dt::FollowPathComponent::LOOP, "path"));
    path->SetFollowRotation(false);
    path->SetSmoothAcceleration(true);
    path->SetSmoothCorners(false);
    std::deque<Ogre::Vector3> straight_path = mNavManager->FindPath(meshnode->GetPosition(), Ogre::Vector3(-2, 0, 6));
    for(std::deque<Ogre::Vector3>::iterator it = straight_path.begin(); it != straight_path.end(); ++it) {
        path->AddPoint(*it);
    }
    path->SetDuration(1.5);
    dt::MeshComponent* mesh = new dt::MeshComponent("Sinbad.mesh", "", "lolmesh");
    meshnode->AddComponent(mesh);
}

}
