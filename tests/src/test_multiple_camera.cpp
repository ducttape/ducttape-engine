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
                dt::Root::get_mutable_instance().GetDisplayManager()->ShowViewport("newView");
                cameraChanged5 = true;
            }
            if(mRuntime > 4.0 && !cameraChanged4) {
                dt::Root::get_mutable_instance().GetDisplayManager()->HideViewport("newView");
                cameraChanged4 = true;
            }
            if(mRuntime > 3.0 && !cameraChanged3) {
                dt::Root::get_mutable_instance().GetDisplayManager()->ActivateCamera("new", "main");
                dt::Root::get_mutable_instance().GetDisplayManager()->ActivateCamera("cam");
                cameraChanged3 = true;
            }
            if(mRuntime > 2.0 && !cameraChanged2) {
                dt::Root::get_mutable_instance().GetDisplayManager()->AddViewport("newView", "new", true, 0.5F, 0.5F, 0.5F, 0.5F);
                dt::Root::get_mutable_instance().GetDisplayManager()->ActivateCamera("cam", "main");
                cameraChanged2 = true;
            }
            if(mRuntime > 1.0 && !cameraChanged) {
                dt::Root::get_mutable_instance().GetDisplayManager()->ActivateCamera("new");
                cameraChanged = true;
            }
        }
    }

    void OnInitialize() {
        dt::Root::get_mutable_instance().GetEventManager()->AddListener(&mScene);

        dt::Root::get_mutable_instance().GetResourceManager()->AddResourceLocation("sinbad.zip","Zip", true);
        Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

        mScene.AddChildNode(new dt::Node("camnode"));
        mScene.FindChildNode("camnode", false)->AddComponent(new dt::CameraComponent("cam"));
        mScene.FindChildNode("camnode", false)->SetPosition(Ogre::Vector3(0, 5, 10));
        mScene.FindChildNode("camnode", false)->FindComponent<dt::CameraComponent>("cam")->LookAt(Ogre::Vector3(0, 0, 0));

        mScene.AddChildNode(new dt::Node("newcam"));
        mScene.FindChildNode("newcam", false)->AddComponent(new dt::CameraComponent("new"));
        mScene.FindChildNode("newcam", false)->SetPosition(Ogre::Vector3(0, 15, 10));
        mScene.FindChildNode("newcam", false)->FindComponent<dt::CameraComponent>("new")->LookAt(Ogre::Vector3(0, 0, 0));

        mScene.AddChildNode(new dt::Node("meshnode"));
        dt::MeshComponent* mesh = new dt::MeshComponent("lolmesh", "Sinbad.mesh");
        mScene.FindChildNode("meshnode", false)->AddComponent(mesh);

        /* std::cout << "Available Animations: ";
        for(std::string s: mesh->GetAvailableAnimations()) {
            std::cout << s << " ";
        }
        std::cout << std::endl; */
        mesh->SetAnimation("Dance");
        mesh->SetLoopAnimation(true);
        mesh->PlayAnimation();

        mScene.AddChildNode(new dt::Node("lightnode"));
        mScene.FindChildNode("lightnode", false)->AddComponent(new dt::LightComponent("light"));
        mScene.FindChildNode("lightnode", false)->SetPosition(Ogre::Vector3(0, 30, 0));

        mScene.AddChildNode(new dt::Node("lightnode2"));
        mScene.FindChildNode("lightnode2", false)->AddComponent(new dt::LightComponent("light2"));
        mScene.FindChildNode("lightnode2", false)->SetPosition(Ogre::Vector3(0, -10, 0));

        //Currently adding a plane would just look horrible till someone will implement the feature to add a material to a mesh or, in this case, a plane.
        /*mScene.AddChildNode(new dt::Node("planenode"));
        mScene.FindChildNode("planenode", false)->AddComponent(new dt::PlaneComponent("plane"));
        mScene.FindChildNode("planenode", false)->SetPosition(Ogre::Vector3(0, -10, 0));*/
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
