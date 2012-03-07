
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// 

#include "TriggerAreaComponentTest/TriggerAreaComponentTest.hpp"

#include <Scene/Game.hpp>
#include <Core/ResourceManager.hpp>
#include <Logic/ScriptManager.hpp>
#include <Graphics/CameraComponent.hpp>
#include <Logic/TriggerAreaComponent.hpp>
#include <Logic/ScriptComponent.hpp>
#include <Logic/FollowPathComponent.hpp>

namespace TriggerAreaComponentTest {

bool TriggerAreaComponentTest::Run(int argc, char** argv) {
    dt::Game game;
    game.Run(new Main(), argc, argv);
    return true;
}

QString TriggerAreaComponentTest::GetTestName() {
    return "TriggerAreaComponent";
}



Main::Main()
    : mRuntime(0),
      mAreaTriggered(false) {}

void Main::UpdateStateFrame(double simulation_frame_time) {
    mRuntime += simulation_frame_time;

    if(mRuntime > 3.0f && !mAreaTriggered) {
        dt::Logger::Get().Error( "The trigger area was not triggered" );
        exit(1);
    }

    if(mRuntime > 3.5) {
        if(mAreaTriggered) {
            dt::Logger::Get().Info( "Test succeeded" );
        }
        dt::StateManager::Get()->Pop(1);
    }
}

dt::Node* Main::_AddMeshNode(dt::Scene* scene, std::string name, Ogre::Vector3 pos)
{
    dt::Node* meshnode = scene->AddChildNode(new dt::Node(name.c_str()));
    dt::MeshComponent* mesh = new dt::MeshComponent("Sinbad.mesh", "", "meshNode1");
    meshnode->AddComponent(mesh);
    meshnode->SetPosition(pos);

    return meshnode;
}

void Main::AreaTriggered() {
    mAreaTriggered = true;
}

void Main::OnInitialize() {
    dt::Scene* scene = AddScene(new dt::Scene("testscene"));

    dt::ResourceManager::Get()->AddResourceLocation("sinbad.zip","Zip", true);
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

    dt::Node* camnode = scene->AddChildNode(new dt::Node("camnode"));
    camnode->SetPosition(Ogre::Vector3(0, 0, 50));
    camnode->AddComponent(new dt::CameraComponent("cam"))->LookAt(Ogre::Vector3(0, 0, 0));;

    dt::Node* meshnode1 = _AddMeshNode(scene, "meshNode1", Ogre::Vector3(0.0f, 20.0f, 0.0f));

    meshnode1->AddComponent(new dt::PhysicsBodyComponent("meshNode1", "meshBody"));

    btCollisionShape * areaBox = new btBoxShape(btVector3(5.0f, 5.0f, 5.0f));

    dt::Node * triggerAreaNode = scene->AddChildNode(new dt::Node("triggerArea"));
    dt::TriggerAreaComponent * triggerAreaComponent = triggerAreaNode->AddComponent(new dt::TriggerAreaComponent(areaBox, "triggerArea"));
    triggerAreaNode->SetPosition(Ogre::Vector3(0.0f, 0.0f, 0.0f));

    QObject::connect(triggerAreaComponent, SIGNAL(Triggered(dt::TriggerAreaComponent*)), this, SLOT(AreaTriggered()));

}

}
