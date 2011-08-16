
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include "DisplayTest/DisplayTest.hpp"

#include <Event/BeginFrameEvent.hpp>
#include <Scene/StateManager.hpp>
#include <Core/ResourceManager.hpp>
#include <Graphics/CameraComponent.hpp>

namespace DisplayTest {

bool DisplayTest::Run(int argc, char** argv) {
    dt::Game game;
    game.Run(new Main(), argc, argv);
    return true;
}

QString DisplayTest::GetTestName() {
    return "Display";
}

////////////////////////////////////////////////////////////////

Main::Main()
    : mRuntime(0) {}

void Main::HandleEvent(std::shared_ptr<dt::Event> e) {
    if(e->GetType() == "DT_BEGINFRAMEEVENT") {
        mRuntime += std::dynamic_pointer_cast<dt::BeginFrameEvent>(e)->GetFrameTime();
        if(mRuntime > 2.5) {
            dt::StateManager::Get()->Pop(1);
        }
    }
}

void Main::OnInitialize() {
    dt::Scene* scene = AddScene(new dt::Scene("testscene"));

    dt::ResourceManager::Get()->AddResourceLocation("sinbad.zip","Zip", true);
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

    dt::Node* camnode = scene->AddChildNode(new dt::Node("camnode"));
    camnode->SetPosition(Ogre::Vector3(0, 5, 10));
    camnode->AddComponent(new dt::CameraComponent("cam"))->LookAt(Ogre::Vector3(0, 0, 0));

    dt::Node* meshnode = scene->AddChildNode(new dt::Node("meshnode"));
    dt::MeshComponent* mesh = new dt::MeshComponent("Sinbad.mesh");
    meshnode->AddComponent(mesh);
    mesh->SetAnimation("Dance");
    mesh->SetLoopAnimation(true);
    mesh->PlayAnimation();
    mesh->SetCastShadows(false);

    dt::Node* lightnode = scene->AddChildNode(new dt::Node("lightnode"));
    lightnode->AddComponent(new dt::LightComponent("light"));
    lightnode->SetPosition(Ogre::Vector3(0, 30, 0));

    dt::Node* lightnode2 = scene->AddChildNode(new dt::Node("lightnode2"));
    lightnode2->AddComponent(new dt::LightComponent("light2"));
    lightnode2->SetPosition(Ogre::Vector3(0, -10, 0));
}

}
