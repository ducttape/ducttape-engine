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
            if(mRuntime > 2000) {
                std::cout << "Request shutdown." << std::endl;
                RequestShutdown();
            }
        }
    }

    void OnInitialize() {
        dt::Root::get_mutable_instance().GetResourceManager()->AddResourceLocation("../data/sinbad.zip","Zip", true);
        //dt::Root::get_mutable_instance().GetResourceManager()->AddResourceLocation("../data/sinbad/","FileSystem", true);
        Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
        dt::Logger::Get().Info("Added resource location");

        dt::Root::get_mutable_instance().GetEventManager()->AddListener(this);

        dt::Node* node = new dt::Node("camnode");
        mScene.AddChildNode(node);
        mScene.FindChildNode("camnode", false)->AddComponent(new dt::CameraComponent("cam"));

        mScene.FindChildNode("camnode", false)->AddComponent(new dt::MeshComponent("lolmesh", "Sinbad.mesh"));
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
