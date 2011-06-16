#include "Root.hpp"
#include "game/Game.hpp"
#include "scene/Scene.hpp"
#include "scene/Node.hpp"
#include "component/MeshComponent.hpp"
#include "event/EventListener.hpp"

class Game : public dt::Game, public dt::EventListener {
public:
    Game()
        : mScene("gamescene") {
        mRuntime = 0;
    }

    void HandleEvent(dt::Event* e) {
        if(e->GetType() == "DT_BEGINFRAMEEVENT") {
            mRuntime += ((dt::BeginFrameEvent*)e)->GetFrameTime();
            if(mRuntime > 5000) {
                RequestShutdown();
            }
        }
    }

    void OnInitialize() {
        dt::Root::get_mutable_instance().GetEventManager()->AddListener(&mScene);

        dt::Root::get_mutable_instance().GetResourceManager()->AddResourceLocation("../data/sinbad.zip","Zip", true);
        Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

        dt::Root::get_mutable_instance().GetEventManager()->AddListener(this);

        mScene.AddChildNode(new dt::Node("camnode"));
        mScene.FindChildNode("camnode", false)->AddComponent(new dt::CameraComponent("cam"));
        mScene.FindChildNode("camnode", false)->SetPosition(Ogre::Vector3(0, 0, 20));

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
    }

private:
    uint32_t mRuntime;
    dt::Scene mScene;

};

int main() {
    Game g;
    g.Run();
    return 0;
}
