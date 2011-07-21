#include "Root.hpp"
#include "game/Game.hpp"
#include "scene/Scene.hpp"
#include "scene/Node.hpp"
#include "component/MeshComponent.hpp"
#include "component/PlaneComponent.hpp"
#include "component/LightComponent.hpp"
#include "event/EventListener.hpp"

class Game : public dt::Game {
public:
    Game()
        : mScene("gamescene") {
        mRuntime = 0;
        cameraChanged = false;
        cameraChanged2 = false;
        cameraChanged3 = false;
        cameraChanged4 = false;
        cameraChanged5 = false;
    }

    void HandleEvent(std::shared_ptr<dt::Event> e) {
        dt::Game::HandleEvent(e);

        if(e->GetType() == "DT_BEGINFRAMEEVENT") {
            mRuntime += std::dynamic_pointer_cast<dt::BeginFrameEvent>(e)->GetFrameTime();
            if(mRuntime > 6.0) {
                RequestShutdown();
            }
            if(mRuntime > 5.0 && !cameraChanged5) {
                dt::DisplayManager::Get()->ShowViewport("newView");
                cameraChanged5 = true;
            }
            if(mRuntime > 4.0 && !cameraChanged4) {
                dt::DisplayManager::Get()->HideViewport("newView");
                cameraChanged4 = true;
            }
            if(mRuntime > 3.0 && !cameraChanged3) {
                dt::DisplayManager::Get()->ActivateCamera("new", "main");
                dt::DisplayManager::Get()->ActivateCamera("cam");
                cameraChanged3 = true;
            }
            if(mRuntime > 2.0 && !cameraChanged2) {
                dt::DisplayManager::Get()->AddViewport("newView", "new", true, 0.5F, 0.5F, 0.5F, 0.5F);
                dt::DisplayManager::Get()->ActivateCamera("cam", "main");
                cameraChanged2 = true;
            }
            if(mRuntime > 1.0 && !cameraChanged) {
                dt::DisplayManager::Get()->ActivateCamera("new");
                cameraChanged = true;
            }
        }
    }

    void OnInitialize() {
        dt::EventManager::Get()->AddListener(&mScene);

        dt::ResourceManager::Get()->AddResourceLocation("sinbad.zip","Zip", true);
        Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

        dt::Node* camnode = mScene.AddChildNode(new dt::Node("camnode"));
        camnode->AddComponent(new dt::CameraComponent("cam"));
        camnode->SetPosition(Ogre::Vector3(0, 5, 10));
        camnode->FindComponent<dt::CameraComponent>("cam")->LookAt(Ogre::Vector3(0, 0, 0));

        dt::Node* newcam = mScene.AddChildNode(new dt::Node("newcam"));
        newcam->AddComponent(new dt::CameraComponent("new"));
        newcam->SetPosition(Ogre::Vector3(0, 15, 10));
        newcam->FindComponent<dt::CameraComponent>("new")->LookAt(Ogre::Vector3(0, 0, 0));

        dt::Node* meshnode = mScene.AddChildNode(new dt::Node("meshnode"));
        dt::MeshComponent* mesh = meshnode->AddComponent(new dt::MeshComponent("lolmesh", "Sinbad.mesh"));
        mesh->SetAnimation("Dance");
        mesh->SetLoopAnimation(true);
        mesh->PlayAnimation();

        dt::Node* lightnode = mScene.AddChildNode(new dt::Node("lightnode"));
        lightnode->AddComponent(new dt::LightComponent("light"));
        lightnode->SetPosition(Ogre::Vector3(0, 30, 0));

        dt::Node* lightnode2 = mScene.AddChildNode(new dt::Node("lightnode2"));
        lightnode2->AddComponent(new dt::LightComponent("light2"));
        lightnode2->SetPosition(Ogre::Vector3(0, -10, 0));
    }

private:
    double mRuntime;
    dt::Scene mScene;
    bool cameraChanged;
    bool cameraChanged2;
    bool cameraChanged3;
    bool cameraChanged4;
    bool cameraChanged5;

};

int main(int argc, char** argv) {
    Game g;
    g.Run(argc, argv);
    return 0;
}
