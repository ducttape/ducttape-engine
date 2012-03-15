
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

bool GuiStateTest::run(int argc, char** argv) {
    dt::Game game;
    game.run(new FirstState(), argc, argv);
    return true;
}

QString GuiStateTest::getTestName() {
    return "GuiState";
}

////////////////////////////////////////////////////////////////

void SecondState::onInitialize() {
    //mPopped = false;
    dt::Logger::get().warning("SecondState::OnInitialize");

    dt::Scene::SceneSP scene = addScene(new dt::Scene("scene2"));

    std::shared_ptr<dt::Node> camnode = scene->addChildNode(new dt::Node("camnode"));
    camnode->setPosition(Ogre::Vector3(0, 0, -20));
    camnode->addComponent(new dt::CameraComponent("cam"))->lookAt(Ogre::Vector3(0, 0, 0));;

    std::shared_ptr<dt::Node> meshnode = scene->addChildNode(new dt::Node("meshnode"));
    dt::MeshComponent* mesh = new dt::MeshComponent("Sinbad.mesh");
    meshnode->addComponent(mesh);
    mesh->setAnimation("Dance");
    mesh->setLoopAnimation(true);
    mesh->playAnimation();
    mesh->setCastShadows(false);

    dt::GuiRootWindow& rootWindow = dt::GuiManager::get()->getRootWindow();
    mReturnButton = rootWindow.addChildWidget(new dt::GuiButton("return"));
    mReturnButton->setCaption("Return");
    mReturnButton->setSize(250, 100);
    mReturnButton->setPosition(100, 100);
    mReturnButton->getMyGUIWidget()->eventMouseButtonClick += MyGUI::newDelegate(this, &SecondState::onClick);
}

void SecondState::onDeinitialize() {
    //dt::GuiManager::Get()->GetRootWindow().RemoveChild("return");
}

void SecondState::onClick(MyGUI::Widget* sender) {
    dt::StateManager::get()->pop();
    dt::StateManager::get()->setNewState(new FirstState());
}

void SecondState::updateStateFrame(double simulation_frame_time) {
}

////////////////////////////////////////////////////////////////

void FirstState::onInitialize() {
    dt::Logger::get().warning("FirstState::OnInitialize");
    //dt::InputManager::Get()->SetJailInput(false);

    dt::Scene::SceneSP scene = addScene(new dt::Scene("scene1"));

    dt::ResourceManager::get()->addResourceLocation("","FileSystem", true);
    dt::ResourceManager::get()->addResourceLocation("sinbad.zip","Zip", true);
    dt::ResourceManager::get()->addResourceLocation("gui","FileSystem", true);
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
    Ogre::FontManager::getSingleton().load("DejaVuSans", "General");

    std::shared_ptr<dt::Node> camnode = scene->addChildNode(new dt::Node("camnode"));
    camnode->setPosition(Ogre::Vector3(0, 0, -10));
    camnode->addComponent(new dt::CameraComponent("cam"))->lookAt(Ogre::Vector3(0, 0, 0));;

    std::shared_ptr<dt::Node> textnode = scene->addChildNode(new dt::Node("text"));
    textnode->setPosition(Ogre::Vector3(0, 0, 0));
    std::shared_ptr<dt::TextComponent> text = textnode->addComponent(new dt::TextComponent("First State", "text"));
    text->setFont("DejaVuSans");
    text->setFontSize(64);

    dt::GuiRootWindow& rootWindow = dt::GuiManager::get()->getRootWindow();
    mNextButton = rootWindow.addChildWidget(new dt::GuiButton("next"));
    mNextButton->setCaption("Next");
    mNextButton->setSize(250, 100);
    mNextButton->setPosition(100, 100);
    mNextButton->getMyGUIWidget()->eventMouseButtonClick += MyGUI::newDelegate(this, &FirstState::onClick);
}

void FirstState::onDeinitialize() {
    //dt::GuiManager::Get()->GetRootWindow().RemoveChild("next");
}

void FirstState::onClick(MyGUI::Widget* sender) {
    dt::StateManager::get()->pop();
    dt::StateManager::get()->setNewState(new SecondState());
}

void FirstState::updateStateFrame(double simulation_frame_time) {
}

}
