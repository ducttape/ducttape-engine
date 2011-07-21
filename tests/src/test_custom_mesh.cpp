
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include "Root.hpp"
#include "game/Game.hpp"
#include "scene/Scene.hpp"
#include "scene/Node.hpp"
#include "component/MeshComponent.hpp"
#include "component/FollowPathComponent.hpp"
#include "component/PlaneComponent.hpp"
#include "component/LightComponent.hpp"
#include "event/EventListener.hpp"

class Game : public dt::Game {
public:
    Game()
        : mScene("gamescene") {
        mRuntime = 0;
    }

    void HandleEvent(std::shared_ptr<dt::Event> e) {
        dt::Game::HandleEvent(e);

        if(e->GetType() == "DT_BEGINFRAMEEVENT") {
            mRuntime += std::dynamic_pointer_cast<dt::BeginFrameEvent>(e)->GetFrameTime();
            if(mRuntime > 5.0) {
                RequestShutdown();
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

        mScene.AddChildNode(new dt::Node("spherenode"));
        dt::MeshComponent* sphere_mesh = dt::MeshComponent::SphereMesh(1.0, "spheremesh");
        mScene.FindChildNode("spherenode", false)->AddComponent(sphere_mesh);

        mScene.AddChildNode(new dt::Node("boxnode"));
        mScene.FindChildNode("boxnode", false)->SetPosition(Ogre::Vector3(-3, 0, 0));
        dt::MeshComponent* box_mesh = dt::MeshComponent::BoxMesh(1.0, 2.0, 3.0, "boxmesh");
        mScene.FindChildNode("boxnode", false)->AddComponent(box_mesh);

        mScene.AddChildNode(new dt::Node("planenode"));
        mScene.FindChildNode("planenode", false)->SetPosition(Ogre::Vector3(3, 0, 0));
        dt::MeshComponent* plane_mesh = dt::MeshComponent::PlaneMesh(1.0, 1.0, 1.0, 1.0, 
                                                                     1.0, 1.0, 20, 20, true, 
                                                                     1, 1.0, 1.0, "planemesh");
        mScene.FindChildNode("planenode", false)->AddComponent(plane_mesh);


        mScene.AddChildNode(new dt::Node("lightnode"));
        mScene.FindChildNode("lightnode", false)->AddComponent(new dt::LightComponent("light"));
        mScene.FindChildNode("lightnode", false)->SetPosition(Ogre::Vector3(0, 30, 0));

        mScene.AddChildNode(new dt::Node("lightnode2"));
        mScene.FindChildNode("lightnode2", false)->AddComponent(new dt::LightComponent("light2"));
        mScene.FindChildNode("lightnode2", false)->SetPosition(Ogre::Vector3(0, -10, 0));
    }

private:
    double mRuntime;
    dt::Scene mScene;

};

int main(int argc, char** argv) {
    Game g;
    g.Run(argc, argv);
    return 0;
}
