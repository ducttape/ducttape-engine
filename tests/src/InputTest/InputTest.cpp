
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include "InputTest/InputTest.hpp"

//#include <Event/BeginFrameEvent.hpp>
#include <Input/InputManager.hpp>
#include <Core/ResourceManager.hpp>
#include <Graphics/CameraComponent.hpp>
#include <Scene/StateManager.hpp>

namespace InputTest {

bool InputTest::run(int argc, char** argv) {
    dt::Game game;
    game.run(new Main(), argc, argv);
    return true;
}

QString InputTest::getTestName() {
    return "Input";
}

////////////////////////////////////////////////////////////////

Main::Main()
    : mRuntime(0) {}

void Main::onInitialize() {
    auto scene = addScene(new dt::Scene("testscene"));
    dt::InputManager::get()->setJailInput(true);

    dt::ResourceManager::get()->addResourceLocation("sinbad.zip","Zip", true);
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

    auto camnode = scene->addChildNode(new dt::Node("camnode"));
    camnode->setPosition(Ogre::Vector3(0, 5, 10));
    camnode->addComponent(new dt::CameraComponent("cam"))->lookAt(Ogre::Vector3(0, 0, 0));
    camnode->addComponent(new dt::SimplePlayerComponent("player"));

    auto meshnode = scene->addChildNode(new dt::Node("meshnode"));
    dt::MeshComponent* mesh = new dt::MeshComponent("Sinbad.mesh");
    meshnode->addComponent(mesh);
    mesh->setAnimation("Dance");
    mesh->setLoopAnimation(true);
    mesh->playAnimation();
}

void Main::updateStateFrame(double simulation_frame_time) {
    mRuntime += simulation_frame_time;
    if(mRuntime > 10.0) {
        dt::StateManager::get()->pop(1);
    }
}

}
