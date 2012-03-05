
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
    : mRuntime(0) {}

void Main::UpdateStateFrame(double simulation_frame_time) {
    mRuntime += simulation_frame_time;

    if(mRuntime > 3.0f) 
    {
        dt::StateManager::Get()->Pop(1);
    }
}

dt::Node* Main::_AddMeshNode(dt::Scene* scene, std::string name, Ogre::Vector3 pos)
{
    dt::Node* meshnode = scene->AddChildNode(new dt::Node(name.c_str()));
    dt::MeshComponent* mesh = new dt::MeshComponent("Sinbad.mesh");
    meshnode->AddComponent(mesh);
    meshnode->SetPosition(pos);

    return meshnode;
}

void Main::OnInitialize() {
    dt::Scene* scene = AddScene(new dt::Scene("testscene"));

    dt::ResourceManager::Get()->AddResourceLocation("sinbad.zip","Zip", true);
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

    dt::ScriptManager::Get()->LoadScript("scripts/circular_movement.js");

    dt::Node* camnode = scene->AddChildNode(new dt::Node("camnode"));
    camnode->SetPosition(Ogre::Vector3(0, 0, 50));
    camnode->AddComponent(new dt::CameraComponent("cam"))->LookAt(Ogre::Vector3(0, 0, 0));;

    dt::Node* meshnode1 = _AddMeshNode(scene, "meshNode1", Ogre::Vector3(0.0f, 0.0f, 0.0f));
    dt::Node* meshnode2 = _AddMeshNode(scene, "meshNode2", Ogre::Vector3(-20.0f, 20.0f, 0.0f));
    
    dt::ScriptComponent * script = new dt::ScriptComponent("circular_movement.js", "script");
    meshnode1->AddComponent(script);
    
    dt::FollowPathComponent * followPath = meshnode2->AddComponent(new dt::FollowPathComponent(dt::FollowPathComponent::LOOP, "followPath"));
    followPath->AddPoint(Ogre::Vector3(-15.0f, 15.0f, 0.0f));
    followPath->AddPoint(Ogre::Vector3(15.0f, 15.0f, 0.0f));
    followPath->SetDuration(5.0);
    followPath->SetSpeed(10.0);

    Ogre::AxisAlignedBox areaBox = Ogre::AxisAlignedBox(Ogre::Vector3(-5.0f, -5.0f, -5.0f), Ogre::Vector3(5.0f, 5.0f, 5.0f));

    dt::Node * triggerAreaNode = scene->AddChildNode(new dt::Node("triggerArea"));
    dt::TriggerAreaComponent * triggerAreaComponent = triggerAreaNode->AddComponent(new dt::TriggerAreaComponent(script, meshnode2, areaBox, "triggerArea"));
    triggerAreaNode->SetPosition(Ogre::Vector3(0.0f, 15.0f, 0.0f));

}

}
