
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include "GuiStateTest/GuiStateTest.hpp"

#include <Scene/StateManager.hpp>
#include <Core/ResourceManager.hpp>

namespace GuiStateTest {

bool GuiStateTest::Run(int argc, char** argv) {
    dt::Game game;
    game.Run(new FirstState(), argc, argv);
    return true;
}

QString GuiStateTest::GetTestName() {
    return "GuiState";
}

////////////////////////////////////////////////////////////////

void SecondState::OnInitialize() {
    //mPopped = false;
    dt::Logger::Get().Warning("SecondState::OnInitialize");

    dt::Scene* scene = AddScene(new dt::Scene("scene2"));

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

    dt::GuiRootWindow& rootWindow = dt::GuiManager::Get()->GetRootWindow();
    mReturnButton = rootWindow.CreateChild<dt::GuiButton>("return");
    mReturnButton->SetCaption("Return");
    mReturnButton->SetSize(250, 100);
    mReturnButton->SetPosition(100, 100);
    dynamic_cast<MyGUI::Button*>(mReturnButton->GetMyGUIWidget())->eventMouseButtonClick
        += MyGUI::newDelegate(this, &SecondState::OnClick);
}

void SecondState::OnDeinitialize() {
    //dt::GuiManager::Get()->GetRootWindow().RemoveChild("return");
}

void SecondState::OnClick(MyGUI::Widget* sender) {
    dt::StateManager::Get()->Pop();
    dt::StateManager::Get()->SetNewState(new FirstState());
}

void SecondState::UpdateStateFrame(double simulation_frame_time) {
}

////////////////////////////////////////////////////////////////

void FirstState::OnInitialize() {
    dt::Logger::Get().Warning("FirstState::OnInitialize");
    //dt::InputManager::Get()->SetJailInput(false);

    dt::Scene* scene = AddScene(new dt::Scene("scene1"));

    dt::ResourceManager::Get()->AddResourceLocation("","FileSystem", true);
    dt::ResourceManager::Get()->AddResourceLocation("sinbad.zip","Zip", true);
    dt::ResourceManager::Get()->AddResourceLocation("gui","FileSystem", true);
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

    dt::GuiRootWindow& rootWindow = dt::GuiManager::Get()->GetRootWindow();
    mNextButton = rootWindow.CreateChild<dt::GuiButton>("next");
    mNextButton->SetCaption("Next");
    mNextButton->SetSize(250, 100);
    mNextButton->SetPosition(100, 100);
    dynamic_cast<MyGUI::Button*>(mNextButton->GetMyGUIWidget())->eventMouseButtonClick
        += MyGUI::newDelegate(this, &FirstState::OnClick);
}

void FirstState::OnDeinitialize() {
    //dt::GuiManager::Get()->GetRootWindow().RemoveChild("next");
}

void FirstState::OnClick(MyGUI::Widget* sender) {
    dt::StateManager::Get()->Pop();
    dt::StateManager::Get()->SetNewState(new SecondState());
}

void FirstState::UpdateStateFrame(double simulation_frame_time) {
}

}
