
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include "TerrainTest/TerrainTest.hpp"

#include <Utils/Logger.hpp>
#include <Scene/StateManager.hpp>
#include <Core/ResourceManager.hpp>
#include <Graphics/CameraComponent.hpp>
#include <Graphics/TerrainManager.hpp>

namespace TerrainTest {

bool TerrainTest::run(int argc, char** argv) {
    dt::Game game;
    game.run(new Main(), argc, argv);
    return true;
}

QString TerrainTest::getTestName() {
    return "Terrain";
}

////////////////////////////////////////////////////////////////

Main::Main()
    : mRuntime(0),
      mBuilding(true) {}

void Main::updateStateFrame(double simulation_frame_time) {
    if(!mBuilding) {
        mRuntime += simulation_frame_time;
    }
    if(mRuntime > 3.0) {
        dt::StateManager::get()->pop(1);
    }
    if(mBuilding && !dt::TerrainManager::get()->getOgreTerrainGroup()->isDerivedDataUpdateInProgress()) {
        mBuilding = false;
        dt::Logger::get().info("Building lightmap complete.");
    }
}

void Main::onInitialize() {
    dt::Scene* scene = addScene(new dt::Scene("testscene"));

    dt::ResourceManager::get()->addResourceLocation("sinbad.zip","Zip", true);
    dt::ResourceManager::get()->addResourceLocation("", "FileSystem", true);
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

    dt::Node* camnode = scene->addChildNode(new dt::Node("camnode"));
    camnode->setPosition(Ogre::Vector3(0, 350, 150));
    dt::CameraComponent* cam = camnode->addComponent(new dt::CameraComponent("cam"));
    cam->lookAt(Ogre::Vector3(0, 300, 0));

    dt::Node* lightnode = scene->addChildNode(new dt::Node("lightnode"));
    dt::LightComponent* light = lightnode->addComponent(new dt::LightComponent("light"));
    lightnode->setPosition(Ogre::Vector3(0, 500, 0));
    lightnode->lookAt(Ogre::Vector3(200, 450, 100));
    light->setColor(Ogre::ColourValue(0.8, 0.6, 0.4, 1.0));

    dt::Node* lightnode2 = scene->addChildNode(new dt::Node("lightnode2"));
    lightnode2->addComponent(new dt::LightComponent("light2"));
    lightnode2->setPosition(Ogre::Vector3(0, 700, -100));

    std::vector<QString> terrain_files;
    terrain_files.push_back("terrain.png");
    std::vector<QString> texture_files;
    texture_files.push_back("pebbles_diffusespecular.jpg");
    texture_files.push_back("pebbles_normalheight.jpg");

    dt::TerrainManager* terrain = dt::TerrainManager::get();
    terrain->setScene(scene);
    terrain->setLight(light);
    terrain->addTextureLayer(texture_files, 100, 0, 0);
    terrain->setScale(300.0f);
    terrain->import(terrain_files);

    dt::Node* meshnode = scene->addChildNode(new dt::Node("meshnode"));
    meshnode->setPosition(Ogre::Vector3(0, 300, 0));
    dt::MeshComponent* mesh = new dt::MeshComponent("Sinbad.mesh");
    meshnode->addComponent(mesh);
}

}
