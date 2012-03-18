
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include "BillboardTest/BillboardTest.hpp"

namespace BillboardTest {

bool BillboardTest::run(int argc, char** argv) {
    dt::Game game;
    game.run(new Main(), argc, argv);
    return true;
}

QString BillboardTest::getTestName() {
    return "Billboard";
}

////////////////////////////////////////////////////////////////

Main::Main()
    : mRuntime(0) {}

void Main::updateStateFrame(double simulation_frame_time) {
    mRuntime += simulation_frame_time;
    mCamNode->findComponent<dt::CameraComponent>("cam")->lookAt(Ogre::Vector3(0, 1, 0));
    if(mRuntime > 5.0) {
        dt::StateManager::get()->pop(1);
    }
}

void Main::onInitialize() {
    auto scene = addScene(new dt::Scene("testscene"));

    dt::ResourceManager::get()->addResourceLocation("", "FileSystem");
    dt::ResourceManager::get()->addResourceLocation("crate", "FileSystem");
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

    OgreProcedural::Root::getInstance()->sceneManager = scene->getSceneManager();

    OgreProcedural::SphereGenerator().setRadius(1.f).setUTile(.5f).realizeMesh("Sphere");
    OgreProcedural::PlaneGenerator().setSizeX(10.f).setSizeY(10.f).realizeMesh("Plane");

    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

    mCamNode = scene->addChildNode(new dt::Node("camnode"));
    mCamNode->addComponent(new dt::CameraComponent("cam"));
    mCamNode->setPosition(Ogre::Vector3(10, 10, 10));
    mCamNode->findComponent<dt::CameraComponent>("cam")->lookAt(Ogre::Vector3(0, 0, 0));
    auto path = mCamNode->addComponent(new dt::FollowPathComponent(dt::FollowPathComponent::LOOP, "path"));
    path->setFollowRotation(false);
    path->setSmoothAcceleration(true);
    path->setSmoothCorners(false);
    path->addPoint(Ogre::Vector3(10, 10, 10));
    path->addPoint(Ogre::Vector3(-10, 10, 10));
    path->addPoint(Ogre::Vector3(10, -10, 10));
    path->setDuration(5.0);


    auto cratenode = scene->addChildNode(new dt::Node("spherenode"));
    cratenode->setPosition(Ogre::Vector3(1, 1, 0));
    cratenode->addComponent(new dt::MeshComponent("Crate01.mesh", "", "sphere-mesh"));

    auto billboardnode = scene->addChildNode(new dt::Node("billboardnode"));
    billboardnode->setPosition(Ogre::Vector3(0, 2, 0));
    billboardnode->addComponent(new dt::BillboardSetComponent(
                                    "billboard_test", 1, "ducttape-logo-256x256.png"));
    auto billboardSetComponent = billboardnode->
            findComponent<dt::BillboardSetComponent>("billboard_test");
    Ogre::BillboardSet* billboardSet = billboardSetComponent
                                       ->getOgreBillboardSet();
    billboardSet->setDefaultDimensions(2, 2);

    auto planenode = scene->addChildNode(new dt::Node("planenode"));
    planenode->setPosition(Ogre::Vector3(0, 0, 0));
    planenode->addComponent(new dt::MeshComponent("Plane", "PrimitivesTest/Pebbles", "plane-mesh"));

    auto lightnode1 = scene->addChildNode(new dt::Node("lightnode1"));
    lightnode1->addComponent(new dt::LightComponent("light1"));
    lightnode1->setPosition(Ogre::Vector3(15, 5, 15));
}

} // namespace BillboardTest
