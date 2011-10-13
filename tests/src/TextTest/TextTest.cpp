
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

bool TextTest::Run(int argc, char** argv) {
    dt::Game game;
    game.Run(new Main(), argc, argv);
    return true;
}

QString TextTest::GetTestName() {
    return "Text";
}

////////////////////////////////////////////////////////////////

Main::Main()
    : mRuntime(0) {}

void Main::UpdateStateFrame(double simulation_frame_time) {
    mRuntime += simulation_frame_time;

    dt::Scene* scene = GetScene("testscene");
    dt::Node* node2 = scene->FindChildNode("node2");
    node2->SetPosition(node2->GetPosition() + (Ogre::Vector3(1, 1, 0) * simulation_frame_time));   //Move too fast before so I decrease the speed.

    if(mRuntime > 2.5) {
        dt::StateManager::Get()->Pop(1);
    }
}

void Main::OnInitialize() {
    dt::Scene* scene = AddScene(new dt::Scene("testscene"));

    dt::ResourceManager::Get()->AddResourceLocation("", "FileSystem", true);
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
    Ogre::FontManager::getSingleton().load("DejaVuSans", "General");

    dt::Node* camnode = scene->AddChildNode(new dt::Node("camnode"));
    camnode->SetPosition(Ogre::Vector3(0, 0, 10));
    camnode->AddComponent(new dt::CameraComponent("cam"))->LookAt(Ogre::Vector3(0, 0, 0));

    dt::Node* node1 = scene->AddChildNode(new dt::Node("node1"));
    dt::TextComponent* text1 = node1->AddComponent(new dt::TextComponent("Hello World", "text1"));
    text1->SetBackgroundMaterial("TextOverlayBackground");
    text1->SetColor(Ogre::ColourValue::White);
    text1->SetFont("DejaVuSans");
    text1->SetFontSize(24);
    text1->SetPadding(Ogre::Vector2(20, 20));


    dt::Node* node2 = scene->AddChildNode(new dt::Node("node2"));
    node2->SetPosition(Ogre::Vector3(0, 0, 0));                       //Out of screen before...
    dt::TextComponent* text2 = node2->AddComponent(new dt::TextComponent("Hi there", "text2"));
    text2->SetColor(Ogre::ColourValue(0.5, 0.0, 1.0));
    text2->SetFont("DejaVuSans");
    text2->SetFontSize(32);

    // place test node behind the camera
    dt::Node* node3 = scene->AddChildNode(new dt::Node("node3"));
    node3->SetPosition(Ogre::Vector3(0, 0, 10));
    dt::TextComponent* text3 = node3->AddComponent(new dt::TextComponent("This text should never be visible", "text3"));
    text3->SetColor(Ogre::ColourValue::Red);
    text3->SetFont("DejaVuSans");
    text3->SetFontSize(32);
}

}
