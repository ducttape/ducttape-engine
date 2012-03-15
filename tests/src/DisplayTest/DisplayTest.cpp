
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include "DisplayTest/DisplayTest.hpp"

#include <Scene/StateManager.hpp>
#include <Core/ResourceManager.hpp>
#include <Graphics/CameraComponent.hpp>

namespace DisplayTest {

bool DisplayTest::run(int argc, char** argv) {
    dt::Game game;
    game.run(new Main(), argc, argv);
    return true;
}

QString DisplayTest::getTestName() {
    return "Display";
}

////////////////////////////////////////////////////////////////

Main::Main()
    : mRuntime(0) {}

void Main::onInitialize() {
    dt::Scene::SceneSP scene = addScene(new dt::Scene("testscene"));

    dt::ResourceManager::get()->addResourceLocation("sinbad.zip","Zip", true);
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

    std::shared_ptr<dt::Node> camnode = scene->addChildNode(new dt::Node("camnode"));
    camnode->setPosition(Ogre::Vector3(0, 5, 10));
    camnode->addComponent(new dt::CameraComponent("cam"))->lookAt(Ogre::Vector3(0, 0, 0));

    std::shared_ptr<dt::Node> meshnode = scene->addChildNode(new dt::Node("meshnode"));
    dt::MeshComponent* mesh = new dt::MeshComponent("Sinbad.mesh");
    meshnode->addComponent(mesh);
    mesh->setAnimation("Dance");
    mesh->setLoopAnimation(true);
    mesh->playAnimation();
    mesh->setCastShadows(false);

    std::shared_ptr<dt::Node> lightnode = scene->addChildNode(new dt::Node("lightnode"));
    lightnode->addComponent(new dt::LightComponent("light"));
    lightnode->setPosition(Ogre::Vector3(0, 30, 0));

    std::shared_ptr<dt::Node> lightnode2 = scene->addChildNode(new dt::Node("lightnode2"));
    lightnode2->addComponent(new dt::LightComponent("light2"));
    lightnode2->setPosition(Ogre::Vector3(0, -10, 0));
}

void Main::updateStateFrame(double simulation_frame_time) {
    mRuntime += simulation_frame_time;
    if(mRuntime > 2.5) {
        dt::StateManager::get()->pop(1);
    }
}

}
