
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_TESTS_TEST_MULTIPLE_CAMERA
#define DUCTTAPE_ENGINE_TESTS_TEST_MULTIPLE_CAMERA

#include <Core/Root.hpp>
#include <Event/EventListener.hpp>
#include <Graphics/LightComponent.hpp>
#include <Graphics/MeshComponent.hpp>
#include <Scene/Game.hpp>
#include <Scene/Node.hpp>
#include <Scene/Scene.hpp>

namespace test_multiple_camera {

class Main : public dt::State {
public:
    Main() {
        mRuntime = 0;
        camera_changed1 = false;
        camera_changed2 = false;
        camera_changed3 = false;
        camera_changed4 = false;
        camera_changed5 = false;
    }

    void HandleEvent(std::shared_ptr<dt::Event> e) {
        if(e->GetType() == "DT_BEGINFRAMEEVENT") {
            mRuntime += std::dynamic_pointer_cast<dt::BeginFrameEvent>(e)->GetFrameTime();
            if(mRuntime > 3.0) {
                dt::StateManager::Get()->Pop(1);
            }
            if(mRuntime > 2.5 && !camera_changed5) {
                dt::DisplayManager::Get()->ShowViewport("newView");
                camera_changed5 = true;
            }
            if(mRuntime > 2.0 && !camera_changed4) {
                dt::DisplayManager::Get()->HideViewport("newView");
                camera_changed4 = true;
            }
            if(mRuntime > 1.5 && !camera_changed3) {
                dt::DisplayManager::Get()->ActivateCamera("new", "main");
                dt::DisplayManager::Get()->ActivateCamera("cam");
                camera_changed3 = true;
            }
            if(mRuntime > 1.0 && !camera_changed2) {
                dt::DisplayManager::Get()->AddViewport("newView", "new", true, 0.5F, 0.5F, 0.5F, 0.5F);
                dt::DisplayManager::Get()->ActivateCamera("cam", "main");
                camera_changed2 = true;
            }
            if(mRuntime > 0.5 && !camera_changed1) {
                dt::DisplayManager::Get()->ActivateCamera("new");
                camera_changed1 = true;
            }
        }
    }

    void OnInitialize() {
        dt::Scene* scene = AddScene(new dt::Scene("testscene"));

        dt::ResourceManager::Get()->AddResourceLocation("sinbad.zip","Zip", true);
        Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

        dt::Node* camnode = scene->AddChildNode(new dt::Node("camnode"));
        dt::CameraComponent* cam = camnode->AddComponent(new dt::CameraComponent("cam"));
        camnode->SetPosition(Ogre::Vector3(0, 5, 10));
        cam->LookAt(Ogre::Vector3(0, 0, 0));

        dt::Node* newcam = scene->AddChildNode(new dt::Node("newcam"));
        dt::CameraComponent* cam2 = newcam->AddComponent(new dt::CameraComponent("new"));
        newcam->SetPosition(Ogre::Vector3(0, 15, 10));
        cam2->LookAt(Ogre::Vector3(0, 0, 0));

        dt::Node* meshnode = scene->AddChildNode(new dt::Node("meshnode"));
        dt::MeshComponent* mesh =
            meshnode->AddComponent(new dt::MeshComponent("Sinbad.mesh"));
        mesh->SetAnimation("Dance");
        mesh->SetLoopAnimation(true);
        mesh->PlayAnimation();

        dt::Node* lightnode = scene->AddChildNode(new dt::Node("lightnode"));
        lightnode->AddComponent(new dt::LightComponent("light"));
        lightnode->SetPosition(Ogre::Vector3(0, 30, 0));

        dt::Node* lightnode2 = scene->AddChildNode(new dt::Node("lightnode2"));
        lightnode2->AddComponent(new dt::LightComponent("light2"));
        lightnode2->SetPosition(Ogre::Vector3(0, -10, 0));
    }

private:
    double mRuntime;
    bool camera_changed1;
    bool camera_changed2;
    bool camera_changed3;
    bool camera_changed4;
    bool camera_changed5;

};

int Run(int argc, char** argv) {
    dt::Game game;
    game.Run(new Main(), argc, argv);
    return 0;
}

} // namespace test_multiple_camera

#endif
