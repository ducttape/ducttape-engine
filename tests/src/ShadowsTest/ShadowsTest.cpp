
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include "ShadowsTest/ShadowsTest.hpp"

//#include <Event/BeginFrameEvent.hpp>
#include <Scene/StateManager.hpp>
#include <Core/ResourceManager.hpp>
#include <Graphics/CameraComponent.hpp>

namespace ShadowsTest {

bool ShadowsTest::run(int argc, char** argv) {
    dt::Game game;
    game.run(new Main(), argc, argv);
    return true;
}

QString ShadowsTest::getTestName() {
    return "Shadows";
}

////////////////////////////////////////////////////////////////

Main::Main()
    : mRuntime(0) {}

void Main::updateStateFrame(double simulation_frame_time) {
    mRuntime += simulation_frame_time;
    if(mRuntime > 2.5) {
        dt::StateManager::get()->pop(1);
    }
}

void Main::onInitialize() {
    dt::Scene* scene = addScene(new dt::Scene("testscene"));

    OgreProcedural::Root::getInstance()->sceneManager = scene->getSceneManager();

    dt::ResourceManager::get()->addResourceLocation("","FileSystem");
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

    OgreProcedural::PlaneGenerator().setNumSegX(20).setNumSegY(20).setSizeX(150).setSizeY(150).setUTile(5.0).setVTile(5.0).realizeMesh("Ground");
    dt::Node* node = scene->addChildNode(new dt::Node("node"));
    dt::MeshComponent* mesh = new dt::MeshComponent("Ground", "PrimitivesTest/Pebbles", "mesh");
    node->addComponent(mesh);
    node->setPosition(Ogre::Vector3(0,0,0));


    OgreProcedural::SphereGenerator().setRadius(1.f).setUTile(5.).setVTile(5.).realizeMesh("Sphere");
    OgreProcedural::BoxGenerator().setSizeX(2.0).setSizeY(2.f).setSizeZ(2.f).realizeMesh("Box");

    // Test primitive generation
    dt::Node* node1 = scene->addChildNode(new dt::Node("node1"));
    dt::MeshComponent* mesh1 = new dt::MeshComponent("Sphere", "PrimitivesTest/RedBrick", "mesh");
    node1->addComponent(mesh1);
    node1->setPosition(Ogre::Vector3(-2,5,0));

    dt::Node* node2 = scene->addChildNode(new dt::Node("node2"));
    dt::MeshComponent* mesh2 = new dt::MeshComponent("Box", "PrimitivesTest/RedBrick", "mesh");
    mesh2->setCastShadows(false);
    node2->addComponent(mesh2);
    node2->setPosition(Ogre::Vector3(2,5,0));

    dt::Node* camnode = scene->addChildNode(new dt::Node("camnode"));
    camnode->setPosition(Ogre::Vector3(0, 5, 10));
    camnode->addComponent(new dt::CameraComponent("cam"))->lookAt(Ogre::Vector3(0, 2, 0));

    dt::Node* lightnode1 = scene->addChildNode(new dt::Node("lightnode1"));
    dt::LightComponent* light = lightnode1->addComponent(new dt::LightComponent("light1"));
    light->setColor(Ogre::ColourValue::White);
    Ogre::Light* ogl = light->getOgreLight();
    ogl->setType(Ogre::Light::LT_SPOTLIGHT);
    ogl->setSpotlightInnerAngle(Ogre::Degree(30));
    ogl->setSpotlightOuterAngle(Ogre::Degree(40));
    lightnode1->setPosition(Ogre::Vector3(0, 20, 0));
    lightnode1->setDirection(Ogre::Vector3::NEGATIVE_UNIT_Y);

    dt::Node* lightnode2 = scene->addChildNode(new dt::Node("lightnode2"));
    dt::LightComponent* light2 = lightnode2->addComponent(new dt::LightComponent("light2"));
    light2->setColor(Ogre::ColourValue::White);
    Ogre::Light* ogl2 = light->getOgreLight();
    ogl2->setType(Ogre::Light::LT_SPOTLIGHT);
    ogl2->setSpotlightInnerAngle(Ogre::Degree(30));
    ogl2->setSpotlightOuterAngle(Ogre::Degree(40));
    lightnode2->setPosition(Ogre::Vector3(5, 20, 0));
    lightnode2->lookAt(Ogre::Vector3(-2,5,0));
}

void Main::putMeshShadow(const QString meshName, const Ogre::Vector3& position, const QString materialName) {
    dt::Scene* scene = dt::StateManager::get()->getCurrentState()->getScene("testscene");
    dt::Node* node = scene->addChildNode(new dt::Node("" + meshName + "node"));
    dt::MeshComponent* mesh = new dt::MeshComponent(meshName, materialName, meshName);
    node->addComponent(mesh);
    node->setPosition(position);
}

}
