
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_TESTS_TEST_DISPLAY
#define DUCTTAPE_ENGINE_TESTS_TEST_DISPLAY

#include <Core/Root.hpp>
#include <Event/EventListener.hpp>
#include <Graphics/LightComponent.hpp>
#include <Graphics/MeshComponent.hpp>
#include <Scene/Game.hpp>
#include <Scene/Node.hpp>
#include <Scene/Scene.hpp>

namespace test_display {

class Main: public dt::State {
public:
    Main() {
        mRuntime = 0;
    }

    void HandleEvent(std::shared_ptr<dt::Event> e) {
        if(e->GetType() == "DT_BEGINFRAMEEVENT") {
            mRuntime += std::dynamic_pointer_cast<dt::BeginFrameEvent>(e)->GetFrameTime();
            if(mRuntime > 2.5) {
                dt::StateManager::Get()->Pop(1);
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

        dt::Node* meshnode = scene->AddChildNode(new dt::Node("meshnode"));
        dt::MeshComponent* mesh = new dt::MeshComponent("Sinbad.mesh");
        meshnode->AddComponent(mesh);
        mesh->SetAnimation("Dance");
        mesh->SetLoopAnimation(true);
        mesh->PlayAnimation();
        mesh->SetCastShadows(false);

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

int Run(int argc, char** argv) {
    dt::Game game;
    game.Run(new Main(), argc, argv);
    return 0;
}

} // namespace test_display

#endif
