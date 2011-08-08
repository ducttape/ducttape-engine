
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_TESTS_TEST_GUI
#define DUCTTAPE_ENGINE_TESTS_TEST_GUI

#include <Core/Root.hpp>
#include <Graphics/CameraComponent.hpp>
#include <Graphics/MeshComponent.hpp>
#include <Gui/GuiButton.hpp>
#include <Scene/Game.hpp>
#include <Scene/Node.hpp>
#include <Scene/Scene.hpp>

namespace test_gui {

class Main: public dt::State {
public:
    Main() {
        mRuntime = 0;
    }

    void Click(MyGUI::Widget* _sender) {
        dt::Logger::Get().Debug("Clicked! " + QString::fromStdString(_sender->getName()));
        if(_sender->getName() == "Gui.b1") {
            static_cast<MyGUI::Button*>(_sender)->setCaption("Not implemented!");
        } else if(_sender->getName() == "Gui.b2") {
            static_cast<MyGUI::Button*>(_sender)->setCaption("Not implemented!");
        } else if(_sender->getName() == "Gui.b3") {
            static_cast<MyGUI::Button*>(_sender)->setCaption("Not implemented!");
        } else if(_sender->getName() == "Gui.b4") {
            dt::StateManager::Get()->Pop(1);
        }
    }

    void HandleEvent(std::shared_ptr<dt::Event> e) {
        if(e->GetType() == "DT_BEGINFRAMEEVENT") {
            mRuntime += std::dynamic_pointer_cast<dt::BeginFrameEvent>(e)->GetFrameTime();
            if(mRuntime > 3.0) {
                dt::StateManager::Get()->Pop(1);
            }
        }
    }

    void OnInitialize() {
        dt::Scene* scene = AddScene(new dt::Scene("testscene"));

        dt::ResourceManager::Get()->AddResourceLocation("sinbad.zip","Zip", true);
        dt::ResourceManager::Get()->AddResourceLocation("gui","FileSystem", true);
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

        // GUI
        dt::GuiRootWindow& win = dt::GuiManager::Get()->GetRootWindow();

        dt::GuiButton* button1 = win.CreateChild<dt::GuiButton>("b1");
        button1->SetCaption("Campaign");
        button1->SetPosition(10, 10);
        button1->SetSize(200, 30);
        dynamic_cast<MyGUI::Button*>(button1->GetMyGUIWidget())->eventMouseButtonClick += MyGUI::newDelegate(this, &Main::Click);

        dt::GuiButton* button2 = win.CreateChild<dt::GuiButton>("b2");
        button2->SetCaption("Tutorial");
        button2->SetPosition(10, 50);
        button2->SetSize(200, 30);
        dynamic_cast<MyGUI::Button*>(button2->GetMyGUIWidget())->eventMouseButtonClick += MyGUI::newDelegate(this, &Main::Click);

        dt::GuiButton* button3 = win.CreateChild<dt::GuiButton>("b3");
        button3->SetCaption("Options");
        button3->SetPosition(10, 90);
        button3->SetSize(200, 30);
        dynamic_cast<MyGUI::Button*>(button3->GetMyGUIWidget())->eventMouseButtonClick += MyGUI::newDelegate(this, &Main::Click);

        dt::GuiButton* button4 = win.CreateChild<dt::GuiButton>("b4");
        button4->SetCaption("Exit");
        button4->SetPosition(10, 130);
        button4->SetSize(200, 30);
        dynamic_cast<MyGUI::Button*>(button4->GetMyGUIWidget())->eventMouseButtonClick += MyGUI::newDelegate(this, &Main::Click);
    }

private:
    double mRuntime;

};

int Run(int argc, char** argv) {
    dt::Game game;
    game.Run(new Main(), argc, argv);
    return 0;
}

} // namespace test_gui

#endif
