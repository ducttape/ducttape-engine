
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include "TextTest/TextTest.hpp"

#include <Scene/StateManager.hpp>
#include <Core/ResourceManager.hpp>

namespace TextTest {

bool TextTest::run(int argc, char** argv) {
    dt::Game game;
    game.run(new Main(), argc, argv);
    return true;
}

QString TextTest::getTestName() {
    return "Text";
}

////////////////////////////////////////////////////////////////

Main::Main()
    : mRuntime(0) {}

void Main::updateStateFrame(double simulation_frame_time) {
    mRuntime += simulation_frame_time;

    auto scene = getScene("testscene");
    auto node2 = scene->findChildNode("node2");
    node2->setPosition(node2->getPosition() + (Ogre::Vector3(1, 1, 0) * simulation_frame_time));   //Move too fast before so I decrease the speed.

    if(mRuntime > 2.5) {
        dt::StateManager::get()->pop(1);
    }
}

void Main::onInitialize() {
    auto scene = addScene(new dt::Scene("testscene"));

    dt::ResourceManager::get()->addResourceLocation("", "FileSystem", true);
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
    Ogre::FontManager::getSingleton().load("DejaVuSans", "General");

    auto camnode = scene->addChildNode(new dt::Node("camnode"));
    camnode->setPosition(Ogre::Vector3(0, 0, 10));
    camnode->addComponent(new dt::CameraComponent("cam"))->lookAt(Ogre::Vector3(0, 0, 0));

    auto node1 = scene->addChildNode(new dt::Node("node1"));
    auto text1 = node1->addComponent(new dt::TextComponent("Hello World", "text1"));
    text1->setBackgroundMaterial("TextOverlayBackground");
    text1->setColor(Ogre::ColourValue::White);
    text1->setFont("DejaVuSans");
    text1->setFontSize(24);
    text1->setPadding(Ogre::Vector2(20, 20));


    auto node2 = scene->addChildNode(new dt::Node("node2"));
    node2->setPosition(Ogre::Vector3(0, 0, 0));                       //Out of screen before...
    auto text2 = node2->addComponent(new dt::TextComponent("Hi there", "text2"));
    text2->setColor(Ogre::ColourValue(0.5, 0.0, 1.0));
    text2->setFont("DejaVuSans");
    text2->setFontSize(32);

    // place test node behind the camera
    auto node3 = scene->addChildNode(new dt::Node("node3"));
    node3->setPosition(Ogre::Vector3(0, 0, 10));
    auto text3 = node3->addComponent(new dt::TextComponent("This text should never be visible", "text3"));
    text3->setColor(Ogre::ColourValue::Red);
    text3->setFont("DejaVuSans");
    text3->setFontSize(32);
}

}
