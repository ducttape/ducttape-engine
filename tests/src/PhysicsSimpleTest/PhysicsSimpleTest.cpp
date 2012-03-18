
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include "PhysicsSimpleTest/PhysicsSimpleTest.hpp"

#include <Scene/StateManager.hpp>
#include <Core/ResourceManager.hpp>
#include <Graphics/CameraComponent.hpp>

namespace PhysicsSimpleTest {

bool PhysicsSimpleTest::run(int argc, char** argv) {
    dt::Game game;
    game.run(new Main(), argc, argv);
    return true;
}

QString PhysicsSimpleTest::getTestName() {
    return "PhysicsSimple";
}

////////////////////////////////////////////////////////////////

Main::Main()
    : mRuntime(0) {}

void Main::updateStateFrame(double simulation_frame_time) {
    mRuntime += simulation_frame_time;

    auto testscene = getScene("testscene");
    auto sphere1 = testscene->findChildNode("spherenode")->findComponent<dt::PhysicsBodyComponent>("sphere-body");
    auto sphere2 = testscene->findChildNode("spherenode2")->findComponent<dt::PhysicsBodyComponent>("sphere-body2");

    if(sphere2->isEnabled() && mRuntime > 1.0) {
        // disable and save position
        sphere2->disable();
        mSphere2DisabledPosition = sphere2->getNode()->getPosition();
    } else if(!sphere2->isEnabled()) {
        // check if it moved
        if(mSphere2DisabledPosition != sphere2->getNode()->getPosition()) {
            std::cerr << "The second sphere moved, even though it should be disabled." << std::endl;
            exit(1);
        }
    }


    if(mRuntime >= 3.0 && testscene->getPhysicsWorld()->isEnabled()) {
        mSphere1DisabledPosition = sphere1->getNode()->getPosition();
    }
    if(!testscene->getPhysicsWorld()->isEnabled()) {
        if(mSphere1DisabledPosition != sphere1->getNode()->getPosition()) {
            std::cerr << "The first sphere moved, even though it should be disabled (the whole physics world should be disabled)." << std::endl;
            exit(1);
        }
    }

    testscene->getPhysicsWorld()->setShowDebug(mRuntime > 2.0);
    testscene->getPhysicsWorld()->setEnabled(mRuntime < 3.0);

    if(mRuntime > 5.0) {
        dt::StateManager::get()->pop(1);
    }
}

void Main::onInitialize() {
    auto scene = addScene(new dt::Scene("testscene"));

    dt::ResourceManager::get()->addResourceLocation("","FileSystem");
    dt::ResourceManager::get()->addResourceLocation("crate","FileSystem");
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

    OgreProcedural::Root::getInstance()->sceneManager = scene->getSceneManager();

    OgreProcedural::SphereGenerator().setRadius(1.f).setUTile(.5f).realizeMesh("Sphere");
    OgreProcedural::PlaneGenerator().setSizeX(10.f).setSizeY(10.f).realizeMesh("Plane");

    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

    auto camnode = scene->addChildNode(new dt::Node("camnode"));
    camnode->setPosition(Ogre::Vector3(15, 2, 15));
    camnode->addComponent(new dt::CameraComponent("cam"))->lookAt(Ogre::Vector3(0, 0, 0));;

    auto spherenode = scene->addChildNode(new dt::Node("spherenode"));
    spherenode->setPosition(Ogre::Vector3(0, 10, 0));
    spherenode->addComponent(new dt::MeshComponent("Crate01.mesh", "", "sphere-mesh"));
    spherenode->addComponent(new dt::PhysicsBodyComponent("sphere-mesh", "sphere-body"));

    auto spherenode2 = scene->addChildNode(new dt::Node("spherenode2"));
    spherenode2->setPosition(Ogre::Vector3(2, 10, 0));
    spherenode2->addComponent(new dt::MeshComponent("Sphere", "PrimitivesTest/RedBrick", "sphere-mesh2"));
    spherenode2->addComponent(new dt::PhysicsBodyComponent("sphere-mesh2", "sphere-body2"));

    auto planenode = scene->addChildNode(new dt::Node("planenode"));
    planenode->setPosition(Ogre::Vector3(0, 0, 0));
    Ogre::Quaternion q;
    q.FromAngleAxis(Ogre::Degree(20), Ogre::Vector3::UNIT_X);
    planenode->setRotation(q);
    planenode->addComponent(new dt::MeshComponent("Plane", "PrimitivesTest/Pebbles", "plane-mesh"));
    planenode->addComponent(new dt::PhysicsBodyComponent("plane-mesh", "plane-body"))->setMass(0.f);

    auto lightnode1 = scene->addChildNode(new dt::Node("lightnode1"));
    lightnode1->addComponent(new dt::LightComponent("light1"));
    lightnode1->setPosition(Ogre::Vector3(15, 5, 15));
}

}
