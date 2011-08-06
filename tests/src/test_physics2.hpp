
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_TESTS_TEST_PHYSICS2
#define DUCTTAPE_ENGINE_TESTS_TEST_PHYSICS2

#include <Core/Root.hpp>
#include <Event/EventListener.hpp>
#include <Graphics/LightComponent.hpp>
#include <Graphics/MeshComponent.hpp>
#include <Physics/PhysicsBodyComponent.hpp>
#include <Scene/Game.hpp>
#include <Scene/Node.hpp>
#include <Scene/Scene.hpp>

#include <OgreProcedural.h>

namespace test_physics2 {

class Main : public dt::State {
public:
    Main() {
        mRuntime = 0;
    }

    void HandleEvent(std::shared_ptr<dt::Event> e) {
        if(e->GetType() == "DT_BEGINFRAMEEVENT") {
            mRuntime += std::dynamic_pointer_cast<dt::BeginFrameEvent>(e)->GetFrameTime();

            if(mRuntime > 10.0) {
                dt::StateManager::Get()->Pop(1);
            }
        }
    }

    void OnInitialize() {
        dt::Scene* scene = AddScene(new dt::Scene("testscene"));

        dt::ResourceManager::Get()->AddResourceLocation("","FileSystem");
        dt::ResourceManager::Get()->AddResourceLocation("crate","FileSystem");
        Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

        OgreProcedural::Root::getInstance()->sceneManager = scene->GetSceneManager();

        OgreProcedural::SphereGenerator().setRadius(1.f).setUTile(.5f).realizeMesh("Sphere");
        OgreProcedural::PlaneGenerator().setSizeX(100.f).setSizeY(100.f).setVTile(10.f).setUTile(10.f).realizeMesh("Plane");

        Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

        dt::Node* camnode = scene->AddChildNode(new dt::Node("camnode"));
        camnode->AddComponent(new dt::CameraComponent("cam"));
        camnode->SetPosition(Ogre::Vector3(30, 25, 30));
        camnode->FindComponent<dt::CameraComponent>("cam")->LookAt(Ogre::Vector3(0, 10, 0));

        dt::Node* planenode = scene->AddChildNode(new dt::Node("planenode"));
        planenode->SetPosition(Ogre::Vector3(0, 0, 0));
        planenode->AddComponent(new dt::MeshComponent("Plane", "PrimitivesTest/Pebbles", "plane-mesh"));
        planenode->AddComponent(new dt::PhysicsBodyComponent("plane-mesh", "plane-body"))->SetMass(0.f);

        dt::Node* lightnode1 = scene->AddChildNode(new dt::Node("lightnode1"));
        lightnode1->AddComponent(new dt::LightComponent("light1"));
        lightnode1->SetPosition(Ogre::Vector3(15, 5, 15));

        int n = 2; // (n*2+1) ^ 3 blocks
        for(int x = -n; x <= n; ++x) {
            for(int y = -n; y <= n; ++y) {
                for(int i = 0; i < n*2 + 1; ++i) {
                    dt::Node* node = scene->AddChildNode(new dt::Node("node"
                                "x-" + dt::Utils::ToString(x) + "-" +
                                "y-" + dt::Utils::ToString(y) + "-" +
                                "z-" + dt::Utils::ToString(i) ));
                    node->SetPosition(Ogre::Vector3(x * 2.5, i * 2.5 + 5, y * 2.5));
                    node->AddComponent(new dt::MeshComponent("Crate01.mesh", "", "mesh"));
                    node->AddComponent(new dt::PhysicsBodyComponent("mesh", "body"));
                }
            }
        }
    }

private:
    double mRuntime;
    Ogre::Vector3 mSphere1DisabledPosition;
    Ogre::Vector3 mSphere2DisabledPosition;

};

int Run(int argc, char** argv) {
    dt::Game game;
    game.Run(new Main(), argc, argv);
    return 0;
}

} // namespace test_physics

#endif
