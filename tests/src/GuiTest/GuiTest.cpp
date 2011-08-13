
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include "GuiTest/GuiTest.hpp"

#include <Event/BeginFrameEvent.hpp>
#include <Scene/StateManager.hpp>
#include <Core/ResourceManager.hpp>
#include <Graphics/CameraComponent.hpp>
#include <Gui/GuiManager.hpp>

namespace GuiTest {

bool GuiTest::Run(int argc, char** argv) {
    dt::Game game;
    game.Run(new Main(), argc, argv);
    return true;
}

QString GuiTest::GetTestName() {
    return "Gui";
}

////////////////////////////////////////////////////////////////

Main::Main()
    : mRuntime(0) {}

void Main::Click(MyGUI::Widget* _sender) {
    dt::Logger::Get().Debug("Clicked! " + QString(_sender->getName().c_str()));
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

void Main::HandleEvent(std::shared_ptr<dt::Event> e) {
    if(e->GetType() == "DT_BEGINFRAMEEVENT") {
        mRuntime += std::dynamic_pointer_cast<dt::BeginFrameEvent>(e)->GetFrameTime();
        if(mRuntime > 3.0) {
            dt::StateManager::Get()->Pop(1);
        }
    }
}

void Main::OnInitialize() {
    dt::Scene* scene = AddScene(new dt::Scene("testscene"));

    dt::ResourceManager::Get()->AddResourceLocation("sinbad.zip","Zip", true);
    dt::ResourceManager::Get()->AddResourceLocation("gui","FileSystem", true);
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

    dt::Node* camnode = scene->AddChildNode(new dt::Node("camnode"));
    camnode->SetPosition(Ogre::Vector3(0, 5, 10));
    camnode->AddComponent(new dt::CameraComponent("cam"))->LookAt(Ogre::Vector3(0, 0, 0));;

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

}
