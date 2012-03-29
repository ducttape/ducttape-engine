
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include "ScriptComponentTest/ScriptComponentTest.hpp"

#include <Scene/StateManager.hpp>
#include <Core/ResourceManager.hpp>
#include <Logic/ScriptManager.hpp>
#include <Graphics/CameraComponent.hpp>

namespace ScriptComponentTest {

bool ScriptComponentTest::run(int argc, char** argv) {
    dt::Game game;
    game.run(new Main(), argc, argv);
    return true;
}

QString ScriptComponentTest::getTestName() {
    return "ScriptComponent";
}

////////////////////////////////////////////////////////////////

Main::Main()
    : mRuntime(0) {}

void Main::updateStateFrame(double simulation_frame_time) {
    mRuntime += simulation_frame_time;
    if(mRuntime > 2) {
        dt::StateManager::get()->pop(1);
    }
}

void Main::onInitialize() {
    auto scene = addScene(new dt::Scene("testscene"));

    dt::ResourceManager::get()->addResourceLocation("sinbad.zip","Zip", true);
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

    dt::ScriptManager::get()->loadScript("scripts/circular_movement.js");

    auto camnode = scene->addChildNode(new dt::Node("camnode"));
    camnode->setPosition(Ogre::Vector3(0, 5, 10));
    camnode->addComponent(new dt::CameraComponent("cam"))->lookAt(Ogre::Vector3(0, 0, 0));;

    auto meshnode = scene->addChildNode(new dt::Node("meshnode"));
    dt::MeshComponent* mesh = new dt::MeshComponent("Sinbad.mesh");
    meshnode->addComponent(mesh);
    meshnode->addComponent(new dt::ScriptComponent("circular_movement.js", "script", true));
}


}
