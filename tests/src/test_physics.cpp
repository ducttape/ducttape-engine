
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include <OgreProcedural.h>

#include "Root.hpp"
#include "game/Game.hpp"
#include "scene/Scene.hpp"
#include "scene/Node.hpp"
#include "component/MeshComponent.hpp"
#include "component/PhysicsBodyComponent.hpp"
#include "component/FollowPathComponent.hpp"
#include "component/LightComponent.hpp"
#include "event/EventListener.hpp"

class Main : public dt::State {
public:
    Main() {
        mRuntime = 0;
    }

    void HandleEvent(std::shared_ptr<dt::Event> e) {
        if(e->GetType() == "DT_BEGINFRAMEEVENT") {
            double last_frame_time =
                std::dynamic_pointer_cast<dt::BeginFrameEvent>(e)->GetFrameTime();

            dt::PhysicsManager::Get()->GetPhysicsWorld()->stepSimulation(last_frame_time, 10);

            mRuntime += last_frame_time;
            if(mRuntime > 5.0) {
                dt::StateManager::Get()->Pop(1);
            }
        }
    }

    void OnInitialize() {
        dt::Scene* scene = AddScene(new dt::Scene("testscene"));

        OgreProcedural::Root::getInstance()->sceneManager = scene->GetSceneManager();

        OgreProcedural::SphereGenerator().setRadius(5.f).setUTile(.5f).realizeMesh("spheremesh");
        OgreProcedural::PlaneGenerator().setSizeX(5.f).setSizeY(5.f).realizeMesh("planemesh");

        Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

        dt::Node* camnode = scene->AddChildNode(new dt::Node("camnode"));
        camnode->AddComponent(new dt::CameraComponent("cam"));
        camnode->SetPosition(Ogre::Vector3(0, 50, 0));
        camnode->FindComponent<dt::CameraComponent>("cam")->LookAt(Ogre::Vector3(0, 0, 0));

        dt::Node* spherenode = scene->AddChildNode(new dt::Node("spherenode"));
        spherenode->AddComponent(new dt::MeshComponent("spheremesh", "", "spheremeshcomponent"));
        spherenode->AddComponent(new dt::PhysicsBodyComponent("spheremeshcomponent"));
        spherenode->SetPosition(Ogre::Vector3(0, 0, 0));

        dt::Node* planenode = scene->AddChildNode(new dt::Node("planenode"));
        planenode->AddComponent(new dt::MeshComponent("planemesh", "", "planemeshcomponent"));
        planenode->AddComponent(new dt::PhysicsBodyComponent("planemeshcomponent"));
        planenode->SetPosition(Ogre::Vector3(0, 0, -10));

        dt::Node* lightnode1 = scene->AddChildNode(new dt::Node("lightnode1"));
        lightnode1->AddComponent(new dt::LightComponent("light1"));
        lightnode1->SetPosition(Ogre::Vector3(0, 30, 0));

        dt::Node* lightnode2 = scene->AddChildNode(new dt::Node("lightnode2"));
        lightnode2->AddComponent(new dt::LightComponent("light2"));
        lightnode2->SetPosition(Ogre::Vector3(0, -10, 0));
    }

private:
    double mRuntime;
};

int main(int argc, char** argv) {
    dt::Game game;
    game.Run(new Main(), argc, argv);
    return 0;
}
