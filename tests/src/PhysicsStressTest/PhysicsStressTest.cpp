
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include "PhysicsStressTest/PhysicsStressTest.hpp"

#include <Scene/StateManager.hpp>
#include <Core/ResourceManager.hpp>
#include <Graphics/CameraComponent.hpp>

namespace PhysicsStressTest {

bool PhysicsStressTest::run(int argc, char** argv) {
    dt::Game game;
    game.run(new Main(), argc, argv);
    return true;
}

QString PhysicsStressTest::getTestName() {
    return "PhysicsStress";
}

////////////////////////////////////////////////////////////////

Main::Main()
    : mRuntime(0) {}

void Main::updateStateFrame(double simulation_frame_time) {
    mRuntime += simulation_frame_time;

    if(mRuntime > 10.0) {
        dt::StateManager::get()->pop(1);
    }
}

void Main::onInitialize() {
    dt::Scene::SceneSP scene = addScene(new dt::Scene("testscene"));

    dt::ResourceManager::get()->addResourceLocation("","FileSystem");
    dt::ResourceManager::get()->addResourceLocation("crate","FileSystem");
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

    OgreProcedural::Root::getInstance()->sceneManager = scene->getSceneManager();

    OgreProcedural::SphereGenerator().setRadius(1.f).setUTile(.5f).realizeMesh("Sphere");
    OgreProcedural::PlaneGenerator().setSizeX(100.f).setSizeY(100.f).setVTile(10.f).setUTile(10.f).realizeMesh("Plane");

    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

    std::shared_ptr<dt::Node> camnode = scene->addChildNode(new dt::Node("camnode"));
    camnode->setPosition(Ogre::Vector3(30, 25, 30));
    camnode->addComponent(new dt::CameraComponent("cam"))->lookAt(Ogre::Vector3(0, 10, 0));;

    std::shared_ptr<dt::Node> planenode = scene->addChildNode(new dt::Node("planenode"));
    planenode->setPosition(Ogre::Vector3(0, 0, 0));
    planenode->addComponent(new dt::MeshComponent("Plane", "PrimitivesTest/Pebbles", "plane-mesh"));
    planenode->addComponent(new dt::PhysicsBodyComponent("plane-mesh", "plane-body", 
        dt::PhysicsBodyComponent::CONVEX, 0.0f));

    std::shared_ptr<dt::Node> lightnode1 = scene->addChildNode(new dt::Node("lightnode1"));
    lightnode1->addComponent(new dt::LightComponent("light1"));
    lightnode1->setPosition(Ogre::Vector3(15, 5, 15));

    int n = 2; // (n*2+1) ^ 3 blocks
    for(int x = -n; x <= n; ++x) {
        for(int y = -n; y <= n; ++y) {
            for(int i = 0; i < n*2 + 1; ++i) {
                std::shared_ptr<dt::Node> node = scene->addChildNode(new dt::Node("node"
                            "x-" + dt::Utils::toString(x) + "-" +
                            "y-" + dt::Utils::toString(y) + "-" +
                            "z-" + dt::Utils::toString(i) ));
                node->setPosition(Ogre::Vector3(x * 2.5, i * 2.5 + 5, y * 2.5));
                node->addComponent(new dt::MeshComponent("Crate01.mesh", "", "mesh"));
                node->addComponent(new dt::PhysicsBodyComponent("mesh", "body"));
            }
        }
    }
}

}
