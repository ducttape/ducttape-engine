
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include "TerrainTest/TerrainTest.hpp"

#include <Utils/Logger.hpp>
#include <Event/BeginFrameEvent.hpp>
#include <Scene/StateManager.hpp>
#include <Core/ResourceManager.hpp>
#include <Graphics/CameraComponent.hpp>
#include <Graphics/TerrainManager.hpp>

namespace TerrainTest {

bool TerrainTest::Run(int argc, char** argv) {
    dt::Game game;
    game.Run(new Main(), argc, argv);
    return true;
}

QString TerrainTest::GetTestName() {
    return "Terrain";
}

////////////////////////////////////////////////////////////////

Main::Main()
    : mRuntime(0),
      mBuilding(true) {}

void Main::HandleEvent(std::shared_ptr<dt::Event> e) {
    if(e->GetType() == "DT_BEGINFRAMEEVENT") {
        if(!mBuilding) {
            mRuntime += std::dynamic_pointer_cast<dt::BeginFrameEvent>(e)->GetFrameTime();
        }
        if(mRuntime > 3.0) {
            dt::StateManager::Get()->Pop(1);
        }
        if(mBuilding && !dt::TerrainManager::Get()->GetOgreTerrainGroup()->isDerivedDataUpdateInProgress()) {
            mBuilding = false;
            dt::Logger::Get().Info("Building lightmap complete.");
        }
    }
}

void Main::OnInitialize() {
    dt::Scene* scene = AddScene(new dt::Scene("testscene"));

    dt::ResourceManager::Get()->AddResourceLocation("sinbad.zip","Zip", true);
    dt::ResourceManager::Get()->AddResourceLocation("", "FileSystem", true);
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

    dt::Node* camnode = scene->AddChildNode(new dt::Node("camnode"));
    camnode->SetPosition(Ogre::Vector3(0, 350, 150));
    dt::CameraComponent* cam = camnode->AddComponent(new dt::CameraComponent("cam"));
    cam->LookAt(Ogre::Vector3(0, 300, 0));

    dt::Node* lightnode = scene->AddChildNode(new dt::Node("lightnode"));
    dt::LightComponent* light = lightnode->AddComponent(new dt::LightComponent("light"));
    lightnode->SetPosition(Ogre::Vector3(0, 500, 0));
    lightnode->LookAt(Ogre::Vector3(200, 450, 100));
    light->SetColor(Ogre::ColourValue(0.8, 0.6, 0.4, 1.0));

    dt::Node* lightnode2 = scene->AddChildNode(new dt::Node("lightnode2"));
    lightnode2->AddComponent(new dt::LightComponent("light2"));
    lightnode2->SetPosition(Ogre::Vector3(0, 700, -100));

    std::vector<QString> terrain_files;
    terrain_files.push_back("terrain.png");
    std::vector<QString> texture_files;
    texture_files.push_back("pebbles_diffusespecular.jpg");
    texture_files.push_back("pebbles_normalheight.jpg");

    dt::TerrainManager* terrain = dt::TerrainManager::Get();
    terrain->SetScene(scene);
    terrain->SetLight(light);
    terrain->AddTextureLayer(texture_files, 100, 0, 0);
    terrain->SetScale(300.0f);
    terrain->Import(terrain_files);

    dt::Node* meshnode = scene->AddChildNode(new dt::Node("meshnode"));
    meshnode->SetPosition(Ogre::Vector3(0, 300, 0));
    dt::MeshComponent* mesh = new dt::MeshComponent("Sinbad.mesh");
    meshnode->AddComponent(mesh);
}

}
