
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include "CamerasTest/CamerasTest.hpp"

namespace CamerasTest {

bool CamerasTest::Run(int argc, char** argv) {
   dt::Game game;
   game.Run(new Main(), argc, argv);
   return true;
}

QString CamerasTest::GetTestName() {
    return "Cameras";
}

////////////////////////////////////////////////////////////////

Main::Main()
    : mRuntime(0) {}

void Main::HandleEvent(std::shared_ptr<dt::Event> e) {
    if(e->GetType() == "DT_BEGINFRAMEEVENT") {
        mRuntime += std::dynamic_pointer_cast<dt::BeginFrameEvent>(e)->GetFrameTime();
        if(mRuntime > 3.0) {
            dt::StateManager::Get()->Pop(1);
        }
        if(mRuntime > 2.5 && mStep == 4) {
            mCamera2->Disable();
            ++mStep;
        }
        if(mRuntime > 2.0 && mStep == 3) {
            mCamera1->Enable();
            mCamera1->SetupViewport(0.3, 0.1, 0.4, 0.4);
            ++mStep;
        }
        if(mRuntime > 1.5 && mStep == 2) {
            mCamera1->Disable();
            ++mStep;
        }
        if(mRuntime > 1.0 && mStep == 1) {
            mCamera2->Enable();
            ++mStep;
        }
        if(mRuntime > 0.5 && mStep == 0) {
            mCamera1->Enable();
            ++mStep;
        }
    }
}

void Main::OnInitialize() {
    dt::Scene* scene = AddScene(new dt::Scene("testscene"));

    dt::ResourceManager::Get()->AddResourceLocation("sinbad.zip","Zip", true);
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();


    dt::Node* camnode = scene->AddChildNode(new dt::Node("camera-node-1"));
    camnode->SetPosition(Ogre::Vector3(0, 5, 10));
    mCamera1 = camnode->AddComponent(new dt::CameraComponent("camera-1"));
    mCamera1->LookAt(Ogre::Vector3(0, 0, 0));

    camnode = scene->AddChildNode(new dt::Node("camera-node-2"));
    camnode->SetPosition(Ogre::Vector3(0, 15, 10));
    mCamera2 = camnode->AddComponent(new dt::CameraComponent("camera-2"));
    mCamera2->SetupViewport(0.4, 0.7, 0.2, 0.2);
    mCamera2->LookAt(Ogre::Vector3(0, 0, 0));

    dt::Node* meshnode = scene->AddChildNode(new dt::Node("meshnode"));
    dt::MeshComponent* mesh = meshnode->AddComponent(new dt::MeshComponent("Sinbad.mesh"));
    mesh->SetAnimation("Dance");
    mesh->SetLoopAnimation(true);
    mesh->PlayAnimation();

    dt::Node* lightnode = scene->AddChildNode(new dt::Node("lightnode"));
    lightnode->AddComponent(new dt::LightComponent("light"));
    lightnode->SetPosition(Ogre::Vector3(0, 30, 0));

    dt::Node* lightnode2 = scene->AddChildNode(new dt::Node("lightnode2"));
    lightnode2->AddComponent(new dt::LightComponent("light2"));
    lightnode2->SetPosition(Ogre::Vector3(0, -10, 0));

    mStep = 0;
}

} // namespace CamerasTest
