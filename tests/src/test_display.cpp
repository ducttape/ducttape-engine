
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

class Main: public dt::State {
public:
    Main() {
        mRuntime = 0;
    }

    void HandleEvent(std::shared_ptr<dt::Event> e) {
        if(e->GetType() == "DT_BEGINFRAMEEVENT") {
            mRuntime += std::dynamic_pointer_cast<dt::BeginFrameEvent>(e)->GetFrameTime();
            if(mRuntime > 5.0) {
                dt::StateManager::Get()->Pop(1);
            }
        }
    }

    void OnInitialize() {
        dt::Scene* scene = AddScene(new dt::Scene("testscene"));
        dt::InputManager::Get()->SetJailInput(true);

        dt::ResourceManager::Get()->AddResourceLocation("sinbad.zip","Zip", true);
        Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

        dt::Node* camnode = scene->AddChildNode(new dt::Node("camnode"));
        dt::CameraComponent* cam = camnode->AddComponent(new dt::CameraComponent("cam"));
        camnode->SetPosition(Ogre::Vector3(0, 5, 10));
        cam->LookAt(Ogre::Vector3(0, 0, 0));
        camnode->AddComponent(new dt::SimplePlayerComponent("player"));

        dt::Node* meshnode = scene->AddChildNode(new dt::Node("meshnode"));
        dt::FollowPathComponent* path =
            new dt::FollowPathComponent(dt::FollowPathComponent::ALTERNATING);
        meshnode->AddComponent(path);
        dt::MeshComponent* mesh = new dt::MeshComponent("Sinbad.mesh");
        meshnode->AddComponent(mesh);
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

};

int main(int argc, char** argv) {
    dt::Game game;
    game.Run(new Main(), argc, argv);
    return 0;
}
