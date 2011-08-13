
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include "ShadowsTest/ShadowsTest.hpp"

#include <Event/BeginFrameEvent.hpp>
#include <Scene/StateManager.hpp>
#include <Core/ResourceManager.hpp>
#include <Graphics/CameraComponent.hpp>

namespace ShadowsTest {

bool ShadowsTest::Run(int argc, char** argv) {
    dt::Game game;
    game.Run(new Main(), argc, argv);
    return true;
}

QString ShadowsTest::GetTestName() {
    return "Shadows";
}

////////////////////////////////////////////////////////////////

Main::Main()
    : mRuntime(0) {}

void Main::HandleEvent(std::shared_ptr<dt::Event> e) {
    if(e->GetType() == "DT_BEGINFRAMEEVENT") {
        mRuntime += std::dynamic_pointer_cast<dt::BeginFrameEvent>(e)->GetFrameTime();
        if(mRuntime > 2.5) {
            dt::StateManager::Get()->Pop(1);
        }
    }
}

void Main::OnInitialize() {
    dt::Scene* scene = AddScene(new dt::Scene("testscene"));

    OgreProcedural::Root::getInstance()->sceneManager = scene->GetSceneManager();

    dt::ResourceManager::Get()->AddResourceLocation("","FileSystem");
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

    OgreProcedural::PlaneGenerator().setNumSegX(20).setNumSegY(20).setSizeX(150).setSizeY(150).setUTile(5.0).setVTile(5.0).realizeMesh("Ground");
    dt::Node* node = scene->AddChildNode(new dt::Node("node"));
    dt::MeshComponent* mesh = new dt::MeshComponent("Ground", "PrimitivesTest/Pebbles", "mesh");
    node->AddComponent(mesh);
    node->SetPosition(Ogre::Vector3(0,0,0));


    OgreProcedural::SphereGenerator().setRadius(1.f).setUTile(5.).setVTile(5.).realizeMesh("Sphere");
    OgreProcedural::BoxGenerator().setSizeX(2.0).setSizeY(2.f).setSizeZ(2.f).realizeMesh("Box");

    // Test primitive generation
    dt::Node* node1 = scene->AddChildNode(new dt::Node("node1"));
    dt::MeshComponent* mesh1 = new dt::MeshComponent("Sphere", "PrimitivesTest/RedBrick", "mesh");
    node1->AddComponent(mesh1);
    node1->SetPosition(Ogre::Vector3(-2,5,0));

    dt::Node* node2 = scene->AddChildNode(new dt::Node("node2"));
    dt::MeshComponent* mesh2 = new dt::MeshComponent("Box", "PrimitivesTest/RedBrick", "mesh");
    mesh2->SetCastShadows(false);
    node2->AddComponent(mesh2);
    node2->SetPosition(Ogre::Vector3(2,5,0));

    dt::Node* camnode = scene->AddChildNode(new dt::Node("camnode"));
    camnode->SetPosition(Ogre::Vector3(0, 5, 10));
    camnode->AddComponent(new dt::CameraComponent("cam"))->LookAt(Ogre::Vector3(0, 2, 0));

    dt::Node* lightnode1 = scene->AddChildNode(new dt::Node("lightnode1"));
    dt::LightComponent* light = lightnode1->AddComponent(new dt::LightComponent("light1"));
    light->SetColor(Ogre::ColourValue::White);
    Ogre::Light* ogl = light->GetOgreLight();
    ogl->setType(Ogre::Light::LT_SPOTLIGHT);
    ogl->setSpotlightInnerAngle(Ogre::Degree(30));
    ogl->setSpotlightOuterAngle(Ogre::Degree(40));
    lightnode1->SetPosition(Ogre::Vector3(0, 20, 0));
    lightnode1->SetDirection(Ogre::Vector3::NEGATIVE_UNIT_Y);

    dt::Node* lightnode2 = scene->AddChildNode(new dt::Node("lightnode2"));
    dt::LightComponent* light2 = lightnode2->AddComponent(new dt::LightComponent("light2"));
    light2->SetColor(Ogre::ColourValue::White);
    Ogre::Light* ogl2 = light->GetOgreLight();
    ogl2->setType(Ogre::Light::LT_SPOTLIGHT);
    ogl2->setSpotlightInnerAngle(Ogre::Degree(30));
    ogl2->setSpotlightOuterAngle(Ogre::Degree(40));
    lightnode2->SetPosition(Ogre::Vector3(5, 20, 0));
    lightnode2->LookAt(Ogre::Vector3(-2,5,0));
}

void Main::PutMeshShadow(const QString& meshName, const Ogre::Vector3& position, const QString materialName) {
    dt::Scene* scene = dt::StateManager::Get()->GetCurrentState()->GetScene("testscene");
    dt::Node* node = scene->AddChildNode(new dt::Node("" + meshName + "node"));
    dt::MeshComponent* mesh = new dt::MeshComponent(meshName, materialName, meshName);
    node->AddComponent(mesh);
    node->SetPosition(position);
}

}
