
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include "PrimitivesTest/PrimitivesTest.hpp"

#include <Event/BeginFrameEvent.hpp>
#include <Scene/StateManager.hpp>
#include <Core/ResourceManager.hpp>
#include <Graphics/CameraComponent.hpp>

namespace PrimitivesTest {

bool PrimitivesTest::Run(int argc, char** argv) {
    dt::Game game;
    game.Run(new Main(), argc, argv);
    return true;
}

QString PrimitivesTest::GetTestName() {
    return "Primitives";
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

    // Test primitive generation
    OgreProcedural::PlaneGenerator().setNumSegX(20).setNumSegY(20).setSizeX(150).setSizeY(150).setUTile(5.0).setVTile(5.0).realizeMesh("planeMesh");
    PutMeshShadow("planeMesh", Ogre::Vector3(0,5,-20), "PrimitivesTest/Pebbles");

    OgreProcedural::SphereGenerator().setRadius(2.f).setUTile(5.).setVTile(5.).realizeMesh("sphereMesh");
    PutMeshShadow("sphereMesh", Ogre::Vector3(0,10,0), "PrimitivesTest/RedBrick");

    OgreProcedural::CylinderGenerator().setHeight(3.f).setRadius(1.f).setUTile(3.).realizeMesh("cylinderMesh");
    PutMeshShadow("cylinderMesh", Ogre::Vector3(10,10,0), "PrimitivesTest/RedBrick");

    OgreProcedural::TorusGenerator().setRadius(3.f).setSectionRadius(1.f).setUTile(10.).setVTile(5.).realizeMesh("torusMesh");
    PutMeshShadow("torusMesh", Ogre::Vector3(-10,10,0), "PrimitivesTest/RedBrick");

    OgreProcedural::ConeGenerator().setRadius(2.f).setHeight(3.f).setNumSegBase(36).setNumSegHeight(2).setUTile(3.).realizeMesh("coneMesh");
    PutMeshShadow("coneMesh", Ogre::Vector3(0,10,-10), "PrimitivesTest/RedBrick");

    OgreProcedural::TubeGenerator().setHeight(3.f).setUTile(3.).realizeMesh("tubeMesh");
    PutMeshShadow("tubeMesh", Ogre::Vector3(-10,10,-10), "PrimitivesTest/RedBrick");

    OgreProcedural::BoxGenerator().setSizeX(2.0).setSizeY(4.f).setSizeZ(6.f).realizeMesh("boxMesh");
    PutMeshShadow("boxMesh", Ogre::Vector3(10,10,-10), "PrimitivesTest/RedBrick");

    OgreProcedural::CapsuleGenerator().setHeight(2.f).realizeMesh("capsuleMesh");
    PutMeshShadow("capsuleMesh", Ogre::Vector3(0,10,10), "PrimitivesTest/RedBrick");

    OgreProcedural::TorusKnotGenerator().setRadius(2.f).setSectionRadius(.5f).setUTile(3.f).setNumSegCircle(64).setNumSegSection(16).realizeMesh("torusKnotMesh");
    PutMeshShadow("torusKnotMesh", Ogre::Vector3(-10,10,10), "PrimitivesTest/RedBrick");

    OgreProcedural::IcoSphereGenerator().setRadius(2.).setNumIterations(3).setUTile(5.).setVTile(5.).realizeMesh("icoSphereMesh");
    PutMeshShadow("icoSphereMesh", Ogre::Vector3(10,10,10), "PrimitivesTest/RedBrick");

    OgreProcedural::RoundedBoxGenerator().setSizeX(1.f).setSizeY(5.f).setSizeZ(5.f).setChamferSize(1.f).realizeMesh("roundedBoxMesh");
    PutMeshShadow("roundedBoxMesh", Ogre::Vector3(20,10,10), "PrimitivesTest/RedBrick");

    // create camera and lights
    dt::Node* camnode = scene->AddChildNode(new dt::Node("camnode"));
    camnode->SetPosition(Ogre::Vector3(-30, 20, 30));
    camnode->AddComponent(new dt::CameraComponent("cam"))->LookAt(Ogre::Vector3(0, 0, 0));

    dt::Node* lightnode1 = scene->AddChildNode(new dt::Node("lightnode1"));
    lightnode1->AddComponent(new dt::LightComponent("light1"));
    lightnode1->SetPosition(Ogre::Vector3(0, 30, 0));

    dt::Node* lightnode2 = scene->AddChildNode(new dt::Node("lightnode2"));
    lightnode2->AddComponent(new dt::LightComponent("light2"));
    lightnode2->SetPosition(Ogre::Vector3(0, -10, 0));
}

void Main::PutMeshShadow(const QString& meshName, const Ogre::Vector3& position, const QString materialName) {
    dt::Scene* scene = dt::StateManager::Get()->GetCurrentState()->GetScene("testscene");
    dt::Node* node = scene->AddChildNode(new dt::Node("" + meshName + "node"));
    dt::MeshComponent* mesh = new dt::MeshComponent(meshName, materialName, meshName);
    node->AddComponent(mesh);
    node->SetPosition(position);
}

}
