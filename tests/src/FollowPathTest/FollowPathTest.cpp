
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include "FollowPathTest/FollowPathTest.hpp"

#include <Event/BeginFrameEvent.hpp>
#include <Scene/StateManager.hpp>
#include <Core/ResourceManager.hpp>
#include <Graphics/CameraComponent.hpp>

namespace FollowPathTest {

bool FollowPathTest::Run(int argc, char** argv) {
    dt::Game game;
    game.Run(new Main(), argc, argv);
    return true;
}

QString FollowPathTest::GetTestName() {
    return "FollowPath";
}

////////////////////////////////////////////////////////////////

Main::Main()
    : mRuntime(0) {}

void Main::HandleEvent(std::shared_ptr<dt::Event> e) {
    if(e->GetType() == "DT_BEGINFRAMEEVENT") {
        mRuntime += std::dynamic_pointer_cast<dt::BeginFrameEvent>(e)->GetFrameTime();
        if(mRuntime > 5.0) {
            dt::StateManager::Get()->Pop(1);
        }
    }
}

void Main::OnInitialize() {
    dt::Scene* scene = AddScene(new dt::Scene("testscene"));

    // Load resources
    dt::ResourceManager::Get()->AddResourceLocation("sinbad.zip","Zip", true);
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

    // Create camera
    dt::Node* camnode = scene->AddChildNode(new dt::Node("camnode"));
    camnode->SetPosition(Ogre::Vector3(0, 20, 20));
    camnode->AddComponent(new dt::CameraComponent("cam"))->LookAt(Ogre::Vector3(0, 0, 0));;

    // Create light
    dt::Node* lightnode = scene->AddChildNode(new dt::Node("lightnode"));
    lightnode->AddComponent(new dt::LightComponent("light"));
    lightnode->SetPosition(Ogre::Vector3(0, 30, 0));

    // Sample #1: LOOP, SmoothAcceleration, Sharp corners, No rotation
    dt::Node* meshnode = scene->AddChildNode(new dt::Node("meshnode"));
    meshnode->SetScale(0.3);
    dt::FollowPathComponent* path = meshnode->AddComponent(new dt::FollowPathComponent(dt::FollowPathComponent::LOOP, "path"));
    path->SetFollowRotation(false);
    path->SetSmoothAcceleration(true);
    path->SetSmoothCorners(false);
    path->AddPoint(Ogre::Vector3(5, 0, 5));
    path->AddPoint(Ogre::Vector3(-5, 0, 5));
    path->AddPoint(Ogre::Vector3(-5, 0, -5));
    path->AddPoint(Ogre::Vector3(5, 0, -5));
    path->AddPoint(Ogre::Vector3(5, 0, 5));
    path->SetDuration(1.5);
    dt::MeshComponent* mesh = new dt::MeshComponent("Sinbad.mesh", "", "lolmesh");
    meshnode->AddComponent(mesh);

    // Sample #2: Alternating, Smooth corners, Follow rotation
    dt::Node* meshnode2 = scene->AddChildNode(new dt::Node("meshnode2"));
    meshnode2->SetScale(0.3);
    dt::FollowPathComponent* path2 = meshnode2->AddComponent(new dt::FollowPathComponent(dt::FollowPathComponent::ALTERNATING, "path2"));
    path2->SetFollowRotation(true);
    path2->SetSmoothAcceleration(false);
    path2->SetSmoothCorners(true);
    path2->AddPoint(Ogre::Vector3(4, 0, 4));
    path2->AddPoint(Ogre::Vector3(0, 0, -4));
    path2->AddPoint(Ogre::Vector3(-4, 0, 4));
    path2->SetDuration(2.5);
    dt::MeshComponent* mesh2 = new dt::MeshComponent("Sinbad.mesh", "", "lolmesh2");
    meshnode2->AddComponent(mesh2);

    // Sample #3: Single, Smooth acceleration, Follow rotation, 3D space
    dt::Node* meshnode3 = scene->AddChildNode(new dt::Node("meshnode3"));
    meshnode3->SetScale(0.3);
    dt::FollowPathComponent* path3 = meshnode3->AddComponent(new dt::FollowPathComponent(dt::FollowPathComponent::SINGLE, "path3"));
    path3->SetFollowRotation(false);
    path3->SetSmoothAcceleration(false);
    path3->SetSmoothCorners(true);
    path3->AddPoint(Ogre::Vector3(6,  0, 0));
    path3->AddPoint(Ogre::Vector3(4,  3, 0));
    path3->AddPoint(Ogre::Vector3(2,  5, 0));
    path3->AddPoint(Ogre::Vector3(0,  6, 0));
    path3->AddPoint(Ogre::Vector3(-2, 5, 0));
    path3->AddPoint(Ogre::Vector3(-4, 3, 0));
    path3->AddPoint(Ogre::Vector3(-6, 0, 0));
    path3->SetDuration(4.0);
    dt::MeshComponent* mesh3 = new dt::MeshComponent("Sinbad.mesh", "", "lolmesh3");
    meshnode3->AddComponent(mesh3);
}

}
