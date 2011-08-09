
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_TESTS_TEST_STATES
#define DUCTTAPE_ENGINE_TESTS_TEST_STATES

#include <Core/Root.hpp>
#include <Event/EventListener.hpp>
#include <Scene/Game.hpp>
#include <Scene/Node.hpp>
#include <Scene/Scene.hpp>
#include <Graphics/TextComponent.hpp>

namespace test_states {

class SecondState : public dt::State {
public:
    void HandleEvent(std::shared_ptr<dt::Event> e) {
        if(e->GetType() == "DT_BEGINFRAMEEVENT") {
            if(dt::Root::GetInstance().GetTimeSinceInitialize() > 4.0 && !mPopped) {
                dt::StateManager::Get()->Pop();
                mPopped = true;
            }
        }
    }

    void OnInitialize() {
        mPopped = false;
        dt::Logger::Get().Warning("SecondState::OnInitialize");

        dt::Scene* scene = AddScene(new dt::Scene("testscene"));

        dt::Node* camnode = scene->AddChildNode(new dt::Node("camnode"));
        camnode->SetPosition(Ogre::Vector3(0, 0, -20));
        camnode->AddComponent(new dt::CameraComponent("cam"))->LookAt(Ogre::Vector3(0, 0, 0));;

        dt::Node* meshnode = scene->AddChildNode(new dt::Node("meshnode"));
        dt::MeshComponent* mesh = new dt::MeshComponent("Sinbad.mesh");
        meshnode->AddComponent(mesh);
        mesh->SetAnimation("Dance");
        mesh->SetLoopAnimation(true);
        mesh->PlayAnimation();
        mesh->SetCastShadows(false);

        dt::Node* textnode = scene->AddChildNode(new dt::Node("text"));
        textnode->SetPosition(Ogre::Vector3(0, 0, 0));
        dt::TextComponent* text = textnode->AddComponent(new dt::TextComponent("Second State", "text"));
        text->SetFont("DejaVuSans");
        text->SetFontSize(64);
    }
private:
    bool mPopped;
};


class FirstState : public dt::State {
public:
    FirstState()
        : mCreated(false) {}

    void HandleEvent(std::shared_ptr<dt::Event> e) {
        if(e->GetType() == "DT_BEGINFRAMEEVENT") {
            if(dt::Root::GetInstance().GetTimeSinceInitialize() > 6.0) {
                dt::StateManager::Get()->Pop();
            }

            if(dt::Root::GetInstance().GetTimeSinceInitialize() > 2.0 && !mCreated) {
                dt::StateManager::Get()->SetNewState(new SecondState());
                mCreated = true;
            }
        }
    }

    void OnInitialize() {
        dt::Logger::Get().Warning("FirstState::OnInitialize");
        dt::InputManager::Get()->SetJailInput(false);

        dt::Scene* scene = AddScene(new dt::Scene("testscene"));

        dt::ResourceManager::Get()->AddResourceLocation("","FileSystem", true);
        dt::ResourceManager::Get()->AddResourceLocation("sinbad.zip","Zip", true);
        Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
        Ogre::FontManager::getSingleton().load("DejaVuSans", "General");

        dt::Node* camnode = scene->AddChildNode(new dt::Node("camnode"));
        camnode->SetPosition(Ogre::Vector3(0, 0, -10));
        camnode->AddComponent(new dt::CameraComponent("cam"))->LookAt(Ogre::Vector3(0, 0, 0));;

        dt::Node* textnode = scene->AddChildNode(new dt::Node("text"));
        textnode->SetPosition(Ogre::Vector3(0, 0, 0));
        dt::TextComponent* text = textnode->AddComponent(new dt::TextComponent("First State", "text"));
        text->SetFont("DejaVuSans");
        text->SetFontSize(64);
    }
private:
    bool mCreated;
};

int Run(int argc, char** argv) {
    dt::Game game;
    game.Run(new FirstState(), argc, argv);
    return 0;
}

} // namespace test_physics

#endif
