
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include "StatesTest/StatesTest.hpp"

#include <Scene/StateManager.hpp>
#include <Core/ResourceManager.hpp>
#include <Graphics/CameraComponent.hpp>
#include <Input/InputManager.hpp>

namespace StatesTest {

bool StatesTest::run(int argc, char** argv) {
    dt::Game game;
    game.run(new FirstState(), argc, argv);
    return true;
}

QString StatesTest::getTestName() {
    return "States";
}

////////////////////////////////////////////////////////////////

void SecondState::updateStateFrame(double simulation_frame_time) {
    if(dt::Root::getInstance().getTimeSinceInitialize() > 4.0 && !mPopped) {
        dt::StateManager::get()->pop();
        mPopped = true;
    }
}

void SecondState::onInitialize() {
    mPopped = false;
    dt::Logger::get().warning("SecondState::OnInitialize");

    dt::Scene* scene = addScene(new dt::Scene("testscene"));

    dt::Node* camnode = scene->addChildNode(new dt::Node("camnode"));
    camnode->setPosition(Ogre::Vector3(0, 0, -20));
    camnode->addComponent(new dt::CameraComponent("cam"))->lookAt(Ogre::Vector3(0, 0, 0));;

    dt::Node* meshnode = scene->addChildNode(new dt::Node("meshnode"));
    dt::MeshComponent* mesh = new dt::MeshComponent("Sinbad.mesh");
    meshnode->addComponent(mesh);
    mesh->setAnimation("Dance");
    mesh->setLoopAnimation(true);
    mesh->playAnimation();
    mesh->setCastShadows(false);

    dt::Node* textnode = scene->addChildNode(new dt::Node("text"));
    textnode->setPosition(Ogre::Vector3(0, 0, 0));
    dt::TextComponent* text = textnode->addComponent(new dt::TextComponent("Second State", "text"));
    text->setFont("DejaVuSans");
    text->setFontSize(64);
}

////////////////////////////////////////////////////////////////

FirstState::FirstState()
    : mCreated(false) {}

void FirstState::updateStateFrame(double simulation_frame_time) {
    if(dt::Root::getInstance().getTimeSinceInitialize() > 6.0) {
        dt::StateManager::get()->pop();
    }

    if(dt::Root::getInstance().getTimeSinceInitialize() > 2.0 && !mCreated) {
        dt::StateManager::get()->setNewState(new SecondState());
        mCreated = true;
    }
}

void FirstState::onInitialize() {
    dt::Logger::get().warning("FirstState::OnInitialize");
    dt::InputManager::get()->setJailInput(false);

    dt::Scene* scene = addScene(new dt::Scene("testscene"));

    dt::ResourceManager::get()->addResourceLocation("","FileSystem", true);
    dt::ResourceManager::get()->addResourceLocation("sinbad.zip","Zip", true);
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
    Ogre::FontManager::getSingleton().load("DejaVuSans", "General");

    dt::Node* camnode = scene->addChildNode(new dt::Node("camnode"));
    camnode->setPosition(Ogre::Vector3(0, 0, -10));
    camnode->addComponent(new dt::CameraComponent("cam"))->lookAt(Ogre::Vector3(0, 0, 0));;

    dt::Node* textnode = scene->addChildNode(new dt::Node("text"));
    textnode->setPosition(Ogre::Vector3(0, 0, 0));
    dt::TextComponent* text = textnode->addComponent(new dt::TextComponent("First State", "text"));
    text->setFont("DejaVuSans");
    text->setFontSize(64);
}

}
