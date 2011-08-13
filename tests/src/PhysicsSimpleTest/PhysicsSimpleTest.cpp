
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include "PhysicsSimpleTest/PhysicsSimpleTest.hpp"

#include <Event/BeginFrameEvent.hpp>
#include <Scene/StateManager.hpp>
#include <Core/ResourceManager.hpp>
#include <Graphics/CameraComponent.hpp>

namespace PhysicsSimpleTest {

bool PhysicsSimpleTest::Run(int argc, char** argv) {
    dt::Game game;
    game.Run(new Main(), argc, argv);
    return true;
}

QString PhysicsSimpleTest::GetTestName() {
    return "PhysicsSimple";
}

////////////////////////////////////////////////////////////////

Main::Main()
    : mRuntime(0) {}

Main::Priority Main::GetEventPriority() const {
    // handle event after the nodes have been updated, so we can
    // monitor their final state
    return EventListener::LOWEST;
}

void Main::HandleEvent(std::shared_ptr<dt::Event> e) {
    if(e->GetType() == "DT_BEGINFRAMEEVENT") {
        mRuntime += std::dynamic_pointer_cast<dt::BeginFrameEvent>(e)->GetFrameTime();

        dt::Scene* testscene = GetScene("testscene");
        dt::PhysicsBodyComponent* sphere1 = testscene->FindChildNode("spherenode")->FindComponent<dt::PhysicsBodyComponent>("sphere-body");
        dt::PhysicsBodyComponent* sphere2 = testscene->FindChildNode("spherenode2")->FindComponent<dt::PhysicsBodyComponent>("sphere-body2");

        if(sphere2->IsEnabled() && mRuntime > 1.0) {
            // disable and save position
            sphere2->Disable();
            mSphere2DisabledPosition = sphere2->GetNode()->GetPosition();
        } else if(!sphere2->IsEnabled()) {
            // check if it moved
            if(mSphere2DisabledPosition != sphere2->GetNode()->GetPosition()) {
                std::cerr << "The second sphere moved, even though it should be disabled." << std::endl;
                exit(1);
            }
        }


        if(mRuntime >= 3.0 && testscene->GetPhysicsWorld()->IsEnabled()) {
            mSphere1DisabledPosition = sphere1->GetNode()->GetPosition();
        }
        if(!testscene->GetPhysicsWorld()->IsEnabled()) {
            if(mSphere1DisabledPosition != sphere1->GetNode()->GetPosition()) {
                std::cerr << "The first sphere moved, even though it should be disabled (the whole physics world should be disabled)." << std::endl;
                exit(1);
            }
        }

        testscene->GetPhysicsWorld()->SetShowDebug(mRuntime > 2.0);
        testscene->GetPhysicsWorld()->SetEnabled(mRuntime < 3.0);

        if(mRuntime > 5.0) {
            dt::StateManager::Get()->Pop(1);
        }
    }
}

void Main::OnInitialize() {
    dt::Scene* scene = AddScene(new dt::Scene("testscene"));

    dt::ResourceManager::Get()->AddResourceLocation("","FileSystem");
    dt::ResourceManager::Get()->AddResourceLocation("crate","FileSystem");
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

    OgreProcedural::Root::getInstance()->sceneManager = scene->GetSceneManager();

    OgreProcedural::SphereGenerator().setRadius(1.f).setUTile(.5f).realizeMesh("Sphere");
    OgreProcedural::PlaneGenerator().setSizeX(10.f).setSizeY(10.f).realizeMesh("Plane");

    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

    dt::Node* camnode = scene->AddChildNode(new dt::Node("camnode"));
    camnode->SetPosition(Ogre::Vector3(15, 2, 15));
    camnode->AddComponent(new dt::CameraComponent("cam"))->LookAt(Ogre::Vector3(0, 0, 0));;

    dt::Node* spherenode = scene->AddChildNode(new dt::Node("spherenode"));
    spherenode->SetPosition(Ogre::Vector3(0, 10, 0));
    spherenode->AddComponent(new dt::MeshComponent("Crate01.mesh", "", "sphere-mesh"));
    spherenode->AddComponent(new dt::PhysicsBodyComponent("sphere-mesh", "sphere-body"));

    dt::Node* spherenode2 = scene->AddChildNode(new dt::Node("spherenode2"));
    spherenode2->SetPosition(Ogre::Vector3(2, 10, 0));
    spherenode2->AddComponent(new dt::MeshComponent("Sphere", "PrimitivesTest/RedBrick", "sphere-mesh2"));
    spherenode2->AddComponent(new dt::PhysicsBodyComponent("sphere-mesh2", "sphere-body2"));

    dt::Node* planenode = scene->AddChildNode(new dt::Node("planenode"));
    planenode->SetPosition(Ogre::Vector3(0, 0, 0));
    Ogre::Quaternion q;
    q.FromAngleAxis(Ogre::Degree(20), Ogre::Vector3::UNIT_X);
    planenode->SetRotation(q);
    planenode->AddComponent(new dt::MeshComponent("Plane", "PrimitivesTest/Pebbles", "plane-mesh"));
    planenode->AddComponent(new dt::PhysicsBodyComponent("plane-mesh", "plane-body"))->SetMass(0.f);

    dt::Node* lightnode1 = scene->AddChildNode(new dt::Node("lightnode1"));
    lightnode1->AddComponent(new dt::LightComponent("light1"));
    lightnode1->SetPosition(Ogre::Vector3(15, 5, 15));
}

}
