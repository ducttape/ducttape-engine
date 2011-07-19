
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

class Game : public dt::Game, public dt::EventListener {
public:
    Game()
        : mScene("gamescene") {
        mRuntime = 0;
    }

    void HandleEvent(std::shared_ptr<dt::Event> e) {
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

        dt::Root::get_mutable_instance().GetEventManager()->AddListener(this);

        mScene.AddChildNode(new dt::Node("camnode"));
        mScene.FindChildNode("camnode", false)->AddComponent(new dt::CameraComponent("cam"));
        mScene.FindChildNode("camnode", false)->SetPosition(Ogre::Vector3(0, 5, 10));
        mScene.FindChildNode("camnode", false)->FindComponent<dt::CameraComponent>("cam")->LookAt(Ogre::Vector3(0, 0, 0));

        mScene.AddChildNode(new dt::Node("meshnode"));
        dt::MeshComponent* mesh = new dt::MeshComponent("lolmesh", "Sinbad.mesh");
        mScene.FindChildNode("meshnode", false)->AddComponent(mesh);
        dt::FollowPathComponent* path = new dt::FollowPathComponent("path", dt::FollowPathComponent::ALTERNATING);
        mScene.FindChildNode("meshnode", false)->AddComponent(path);
        path = mScene.FindChildNode("meshnode", false)->FindComponent<dt::FollowPathComponent>("path");

        path->AddPoint(Ogre::Vector3(3, 0, 0));

        for(float x = 2; x >= -3; x -= 1) {
            path->AddPoint(Ogre::Vector3(x + 0.5, 1, 0));
            path->AddPoint(Ogre::Vector3(x, 0, 0));
        }
        path->SetDuration(3.f);
        path->SetSmoothCorners(true);
        path->SetSmoothAcceleration(true);


        /*std::cout << "Available Animations: ";
        for(std::string s: mesh->GetAvailableAnimations()) {
            std::cout << s << " ";
        }
        std::cout << std::endl;*/
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

};

int main(int argc, char** argv) {
    Game g;
    g.Run(argc, argv);
    return 0;
}
