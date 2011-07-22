
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
#include "component/SimplePlayerComponent.hpp"
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
        dt::EventManager::Get()->AddListener(&mScene);
        dt::InputManager::Get()->SetJailInput(true);

        // Load resources
        dt::ResourceManager::Get()->AddResourceLocation("sinbad.zip","Zip", true);
        Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

        // Create camera
        dt::Node* camnode = mScene.AddChildNode(new dt::Node("camnode"));
        camnode->AddComponent(new dt::CameraComponent("cam"));
        camnode->SetPosition(Ogre::Vector3(0, 20, 20));
        camnode->FindComponent<dt::CameraComponent>("cam")->LookAt(Ogre::Vector3(0, 0, 0));
        camnode->AddComponent(new dt::SimplePlayerComponent("player"));

        // Create light
        dt::Node* lightnode = mScene.AddChildNode(new dt::Node("lightnode"));
        lightnode->AddComponent(new dt::LightComponent("light"));
        lightnode->SetPosition(Ogre::Vector3(0, 30, 0));

        // Sample #1: LOOP, SmoothAcceleration, Sharp corners, No rotation
        dt::Node* meshnode = mScene.AddChildNode(new dt::Node("meshnode"));
        meshnode->SetScale(0.3);
        dt::FollowPathComponent* path = meshnode->AddComponent(new dt::FollowPathComponent("path", dt::FollowPathComponent::LOOP));
        path->SetFollowRotation(false);
        path->SetSmoothAcceleration(true);
        path->SetSmoothCorners(false);
        path->AddPoint(Ogre::Vector3(5, 0, 5));
        path->AddPoint(Ogre::Vector3(-5, 0, 5));
        path->AddPoint(Ogre::Vector3(-5, 0, -5));
        path->AddPoint(Ogre::Vector3(5, 0, -5));
        path->AddPoint(Ogre::Vector3(5, 0, 5));
        path->SetDuration(1.5);
        dt::MeshComponent* mesh = new dt::MeshComponent("lolmesh", "Sinbad.mesh");
        meshnode->AddComponent(mesh);

        // Sample #2: Alternating, Smooth corners, Follow rotation
        dt::Node* meshnode2 = mScene.AddChildNode(new dt::Node("meshnode2"));
        meshnode2->SetScale(0.3);
        dt::FollowPathComponent* path2 = meshnode2->AddComponent(new dt::FollowPathComponent("path2", dt::FollowPathComponent::ALTERNATING));
        path2->SetFollowRotation(true);
        path2->SetSmoothAcceleration(false);
        path2->SetSmoothCorners(true);
        path2->AddPoint(Ogre::Vector3(4, 0, 4));
        path2->AddPoint(Ogre::Vector3(0, 0, -4));
        path2->AddPoint(Ogre::Vector3(-4, 0, 4));
        path2->SetDuration(2.5);
        dt::MeshComponent* mesh2 = new dt::MeshComponent("lolmesh2", "Sinbad.mesh");
        meshnode2->AddComponent(mesh2);

        // Sample #3: Single, Smooth acceleration, Follow rotation, 3D space
        dt::Node* meshnode3 = mScene.AddChildNode(new dt::Node("meshnode3"));
        meshnode3->SetScale(0.3);
        dt::FollowPathComponent* path3 = meshnode3->AddComponent(new dt::FollowPathComponent("path3", dt::FollowPathComponent::SINGLE));
        path3->SetFollowRotation(false);
        path3->SetSmoothAcceleration(false);
        path3->SetSmoothCorners(true);
        path3->AddPoint(Ogre::Vector3(6,  0, 0));
        path3->AddPoint(Ogre::Vector3(4,  3, 0));
        path3->AddPoint(Ogre::Vector3(2,  5, 0));
        path3->AddPoint(Ogre::Vector3(0,  6, 0));
        path3->AddPoint(Ogre::Vector3(-2, 5, 0));
        path3->AddPoint(Ogre::Vector3(-4, 3, 0));
        path3->AddPoint(Ogre::Vector3(-6, 0, 0));
        path3->SetDuration(4.0);
        dt::MeshComponent* mesh3 = new dt::MeshComponent("lolmesh3", "Sinbad.mesh");
        meshnode3->AddComponent(mesh3);
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
