
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include "FollowPathTest/FollowPathTest.hpp"

#include <Scene/StateManager.hpp>
#include <Core/ResourceManager.hpp>
#include <Graphics/CameraComponent.hpp>

namespace FollowPathTest {

bool FollowPathTest::run(int argc, char** argv) {
    dt::Game game;
    game.run(new Main(), argc, argv);
    return true;
}

QString FollowPathTest::getTestName() {
    return "FollowPath";
}

////////////////////////////////////////////////////////////////

Main::Main()
    : mRuntime(0) {}

void Main::onInitialize() {
    dt::Scene* scene = addScene(new dt::Scene("testscene"));

    // Load resources
    dt::ResourceManager::get()->addResourceLocation("sinbad.zip","Zip", true);
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

    // Create camera
    dt::Node* camnode = scene->addChildNode(new dt::Node("camnode"));
    camnode->setPosition(Ogre::Vector3(0, 20, 20));
    camnode->addComponent(new dt::CameraComponent("cam"))->lookAt(Ogre::Vector3(0, 0, 0));;

    // Create light
    dt::Node* lightnode = scene->addChildNode(new dt::Node("lightnode"));
    lightnode->addComponent(new dt::LightComponent("light"));
    lightnode->setPosition(Ogre::Vector3(0, 30, 0));

    // Sample #1: LOOP, SmoothAcceleration, Sharp corners, No rotation
    dt::Node* meshnode = scene->addChildNode(new dt::Node("meshnode"));
    meshnode->setScale(0.3);
    dt::FollowPathComponent* path = meshnode->addComponent(new dt::FollowPathComponent(dt::FollowPathComponent::LOOP, "path"));
    path->setFollowRotation(false);
    path->setSmoothAcceleration(true);
    path->setSmoothCorners(false);
    path->addPoint(Ogre::Vector3(5, 0, 5));
    path->addPoint(Ogre::Vector3(-5, 0, 5));
    path->addPoint(Ogre::Vector3(-5, 0, -5));
    path->addPoint(Ogre::Vector3(5, 0, -5));
    path->addPoint(Ogre::Vector3(5, 0, 5));
    path->setDuration(1.5);
    dt::MeshComponent* mesh = new dt::MeshComponent("Sinbad.mesh", "", "lolmesh");
    meshnode->addComponent(mesh);

    // Sample #2: Alternating, Smooth corners, Follow rotation
    dt::Node* meshnode2 = scene->addChildNode(new dt::Node("meshnode2"));
    meshnode2->setScale(0.3);
    dt::FollowPathComponent* path2 = meshnode2->addComponent(new dt::FollowPathComponent(dt::FollowPathComponent::ALTERNATING, "path2"));
    path2->setFollowRotation(true);
    path2->setSmoothAcceleration(false);
    path2->setSmoothCorners(true);
    path2->addPoint(Ogre::Vector3(4, 0, 4));
    path2->addPoint(Ogre::Vector3(0, 0, -4));
    path2->addPoint(Ogre::Vector3(-4, 0, 4));
    path2->setDuration(2.5);
    dt::MeshComponent* mesh2 = new dt::MeshComponent("Sinbad.mesh", "", "lolmesh2");
    meshnode2->addComponent(mesh2);

    // Sample #3: Single, Smooth acceleration, Follow rotation, 3D space
    dt::Node* meshnode3 = scene->addChildNode(new dt::Node("meshnode3"));
    meshnode3->setScale(0.3);
    dt::FollowPathComponent* path3 = meshnode3->addComponent(new dt::FollowPathComponent(dt::FollowPathComponent::SINGLE, "path3"));
    path3->setFollowRotation(false);
    path3->setSmoothAcceleration(false);
    path3->setSmoothCorners(true);
    path3->addPoint(Ogre::Vector3(6,  0, 0));
    path3->addPoint(Ogre::Vector3(4,  3, 0));
    path3->addPoint(Ogre::Vector3(2,  5, 0));
    path3->addPoint(Ogre::Vector3(0,  6, 0));
    path3->addPoint(Ogre::Vector3(-2, 5, 0));
    path3->addPoint(Ogre::Vector3(-4, 3, 0));
    path3->addPoint(Ogre::Vector3(-6, 0, 0));
    path3->setDuration(4.0);
    dt::MeshComponent* mesh3 = new dt::MeshComponent("Sinbad.mesh", "", "lolmesh3");
    meshnode3->addComponent(mesh3);
}

void Main::updateStateFrame(double simulation_frame_time) {
    mRuntime += simulation_frame_time;
    if(mRuntime > 5.0) {
        dt::StateManager::get()->pop(1);
    }
}

}
