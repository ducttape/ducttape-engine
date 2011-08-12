#include "TerrainTest/TerrainTest.hpp"

#include <Input/MouseEvent.hpp>
#include <Logic/SimplePlayerComponent.hpp>

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
    : mRuntime(0) {}

void Main::HandleEvent(std::shared_ptr<dt::Event> e) {
    /*if(e->GetType() == "DT_BEGINFRAMEEVENT") {
        mRuntime += std::dynamic_pointer_cast<dt::BeginFrameEvent>(e)->GetFrameTime();
        if(mRuntime > 2.5) {
            dt::StateManager::Get()->Pop(1);
        }
    }*/

    if(e->GetType() == "DT_MOUSEEVENT") {
        if(std::dynamic_pointer_cast<dt::MouseEvent>(e)->GetAction() == dt::MouseEvent::RELEASED) {
            dt::StateManager::Get()->Pop(1);
        }
    }
}

void Main::OnInitialize() {
    dt::InputManager::Get()->SetJailInput(false);

    dt::Scene* scene = AddScene(new dt::Scene("testscene"));

    dt::ResourceManager::Get()->AddResourceLocation("sinbad.zip","Zip", true);
    dt::ResourceManager::Get()->AddResourceLocation("", "FileSystem", true);
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

    dt::Node* camnode = scene->AddChildNode(new dt::Node("camnode"));
    camnode->SetPosition(Ogre::Vector3(0, 120, 70));
    dt::CameraComponent* cam = camnode->AddComponent(new dt::CameraComponent("cam"));
    cam->LookAt(Ogre::Vector3(0, 120, 0));
    camnode->AddComponent(new dt::SimplePlayerComponent());
    //cam->GetCamera()->setPolygonMode( Ogre::PM_WIREFRAME );
    //dt::DisplayManager::Get()->GetRenderWindow()->getViewport(0)->setBackgroundColour(Ogre::ColourValue::White);

    dt::Node* lightnode = scene->AddChildNode(new dt::Node("lightnode"));
    dt::LightComponent* light = lightnode->AddComponent(new dt::LightComponent("light"));
    lightnode->SetPosition(Ogre::Vector3(0, 300, 0));

    dt::Node* lightnode2 = scene->AddChildNode(new dt::Node("lightnode2"));
    lightnode2->AddComponent(new dt::LightComponent("light2"));
    lightnode2->SetPosition(Ogre::Vector3(0, 100, 0));

    std::vector<QString> terrain_files;
    terrain_files.push_back("terrain.png");
    std::vector<QString> texture_files;
    texture_files.push_back("pebbles.jpg");
    //texture_files.push_back("dirt_grayrocky_normalheight.dds");
    dt::TerrainManager* terrain = dt::TerrainManager::Get();
    terrain->SetScene("testscene");
    terrain->SetLight(light);
    terrain->AddTextureLayer(texture_files, 100, 0, 0);
    terrain->Import(terrain_files);

    dt::Node* meshnode = scene->AddChildNode(new dt::Node("meshnode"));
    meshnode->SetPosition(Ogre::Vector3(0, 120, 0));
    dt::MeshComponent* mesh = new dt::MeshComponent("Sinbad.mesh");
    meshnode->AddComponent(mesh);
}

}
