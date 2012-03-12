
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include "CamerasTest/CamerasTest.hpp"

namespace CamerasTest {

bool CamerasTest::run(int argc, char** argv) {
   dt::Game game;
   game.run(new Main(), argc, argv);
   return true;
}

QString CamerasTest::getTestName() {
    return "Cameras";
}

////////////////////////////////////////////////////////////////

Main::Main()
    : mRuntime(0) {}

void Main::onInitialize() {
    dt::Scene* scene = addScene(new dt::Scene("testscene"));

    dt::ResourceManager::get()->addResourceLocation("sinbad.zip","Zip", true);
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();


    dt::Node* camnode = scene->addChildNode(new dt::Node("camera-node-1"));
    camnode->setPosition(Ogre::Vector3(0, 5, 10));
    mCamera1 = camnode->addComponent(new dt::CameraComponent("camera-1"));
    mCamera1->lookAt(Ogre::Vector3(0, 0, 0));

    camnode = scene->addChildNode(new dt::Node("camera-node-2"));
    camnode->setPosition(Ogre::Vector3(0, 15, 10));
    mCamera2 = camnode->addComponent(new dt::CameraComponent("camera-2"));
    mCamera2->setupViewport(0.4, 0.7, 0.2, 0.2);
    mCamera2->lookAt(Ogre::Vector3(0, 0, 0));

    dt::Node* meshnode = scene->addChildNode(new dt::Node("meshnode"));
    dt::MeshComponent* mesh = meshnode->addComponent(new dt::MeshComponent("Sinbad.mesh"));
    mesh->setAnimation("Dance");
    mesh->setLoopAnimation(true);
    mesh->playAnimation();

    dt::Node* lightnode = scene->addChildNode(new dt::Node("lightnode"));
    lightnode->addComponent(new dt::LightComponent("light"));
    lightnode->setPosition(Ogre::Vector3(0, 30, 0));

    dt::Node* lightnode2 = scene->addChildNode(new dt::Node("lightnode2"));
    lightnode2->addComponent(new dt::LightComponent("light2"));
    lightnode2->setPosition(Ogre::Vector3(0, -10, 0));

    mStep = 0;
}

void Main::updateStateFrame(double simulation_frame_time) {
    mRuntime += simulation_frame_time;
    if(mRuntime > 3.0) {
        dt::StateManager::get()->pop(1);
    }
    if(mRuntime > 2.5 && mStep == 4) {
        mCamera2->disable();
        ++mStep;
    }
    if(mRuntime > 2.0 && mStep == 3) {
        mCamera1->enable();
        mCamera1->setupViewport(0.3, 0.1, 0.4, 0.4);
        ++mStep;
    }
    if(mRuntime > 1.5 && mStep == 2) {
        mCamera1->disable();
        ++mStep;
    }
    if(mRuntime > 1.0 && mStep == 1) {
        mCamera2->enable();
        ++mStep;
    }
    if(mRuntime > 0.5 && mStep == 0) {
        mCamera1->enable();
        ++mStep;
    }
}

} // namespace CamerasTest
