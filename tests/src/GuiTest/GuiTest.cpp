
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include "GuiTest/GuiTest.hpp"

#include <Scene/StateManager.hpp>
#include <Core/ResourceManager.hpp>
#include <Graphics/CameraComponent.hpp>
#include <Gui/GuiManager.hpp>

namespace GuiTest {

bool GuiTest::run(int argc, char** argv) {
    dt::Game game;
    game.run(new Main(), argc, argv);
    return true;
}

QString GuiTest::getTestName() {
    return "Gui";
}

////////////////////////////////////////////////////////////////

Main::Main()
    : mRuntime(0) {}

void Main::click(MyGUI::Widget* _sender) {
    dt::Logger::get().debug("Clicked! " + QString(_sender->getName().c_str()));
    if(_sender->getName() == "Gui.b1") {
        static_cast<MyGUI::Button*>(_sender)->setCaption("Not implemented!");
    } else if(_sender->getName() == "Gui.b2") {
        static_cast<MyGUI::Button*>(_sender)->setCaption("Not implemented!");
    } else if(_sender->getName() == "Gui.b3") {
        static_cast<MyGUI::Button*>(_sender)->setCaption("Not implemented!");
    } else if(_sender->getName() == "Gui.b4") {
        dt::StateManager::get()->pop(1);
    }
}

void Main::onInitialize() {
    auto scene = addScene(new dt::Scene("testscene"));

    dt::ResourceManager::get()->addResourceLocation("sinbad.zip","Zip", true);
    dt::ResourceManager::get()->addResourceLocation("gui","FileSystem", true);
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

    auto camnode = scene->addChildNode(new dt::Node("camnode"));
    camnode->setPosition(Ogre::Vector3(0, 5, 10));
    camnode->addComponent(new dt::CameraComponent("cam"))->lookAt(Ogre::Vector3(0, 0, 0));;

    auto meshnode = scene->addChildNode(new dt::Node("meshnode"));
    dt::MeshComponent* mesh = new dt::MeshComponent("Sinbad.mesh");
    meshnode->addComponent(mesh);
    mesh->setAnimation("Dance");
    mesh->setLoopAnimation(true);
    mesh->playAnimation();

    // GUI
    dt::GuiRootWindow& win = dt::GuiManager::get()->getRootWindow();

    auto button1 = win.addChildWidget(new dt::GuiButton("b1"));
    button1->setCaption("Campaign");
    button1->setPosition(10, 10);
    button1->setSize(200, 30);
    button1->getMyGUIWidget()->eventMouseButtonClick += MyGUI::newDelegate(this, &Main::click);

    auto button2 = win.addChildWidget(new dt::GuiButton("b2"));
    button2->setCaption("Tutorial");
    button2->setPosition(10, 50);
    button2->setSize(200, 30);
    button2->getMyGUIWidget()->eventMouseButtonClick += MyGUI::newDelegate(this, &Main::click);

    auto button3 = win.addChildWidget(new dt::GuiButton("b3"));
    button3->setCaption("Options");
    button3->setPosition(10, 90);
    button3->setSize(200, 30);
    button3->getMyGUIWidget()->eventMouseButtonClick += MyGUI::newDelegate(this, &Main::click);

    auto button4 = win.addChildWidget(new dt::GuiButton("b4"));
    button4->setCaption("Exit");
    button4->setPosition(10, 130);
    button4->setSize(200, 30);
    button4->getMyGUIWidget()->eventMouseButtonClick += MyGUI::newDelegate(this, &Main::click);
}

void Main::updateStateFrame(double simulation_frame_time) {
    mRuntime += simulation_frame_time;
    if(mRuntime > 3.0) {
        dt::StateManager::get()->pop(1);
    }
}

}
