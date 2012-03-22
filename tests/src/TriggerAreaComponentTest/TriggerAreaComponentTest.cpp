
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// 

#include "TriggerAreaComponentTest/TriggerAreaComponentTest.hpp"

#include <Scene/Game.hpp>
#include <Core/ResourceManager.hpp>
#include <Logic/ScriptManager.hpp>
#include <Graphics/CameraComponent.hpp>
#include <Logic/ScriptComponent.hpp>
#include <Logic/FollowPathComponent.hpp>

namespace TriggerAreaComponentTest {

bool TriggerAreaComponentTest::run(int argc, char** argv) {
    dt::Game game;
    game.run(new Main(), argc, argv);
    return true;
}

QString TriggerAreaComponentTest::getTestName() {
    return "TriggerAreaComponent";
}



Main::Main()
    : mRuntime(0),
      mAreaTriggered(false) {}

void Main::updateStateFrame(double simulation_frame_time) {
    mRuntime += simulation_frame_time;

    if(mRuntime > 3.0f && !mAreaTriggered) {
        dt::Logger::get().error( "The trigger area was not triggered" );
        exit(1);
    }

    if(mRuntime > 3.5) {
        if(mAreaTriggered) {
            dt::Logger::get().info( "Test succeeded" );
        }
        dt::StateManager::get()->pop(1);
    }
}

dt::Node::NodeSP Main::_addMeshNode(dt::Scene* scene, std::string name, Ogre::Vector3 pos)
{
    dt::Node::NodeSP meshnode = scene->addChildNode(new dt::Node(name.c_str()));
    dt::MeshComponent* mesh = new dt::MeshComponent("Sinbad.mesh", "", "meshNode1");
    meshnode->addComponent(mesh);
    meshnode->setPosition(pos);

    return meshnode;
}

void Main::areaTriggered(dt::TriggerAreaComponent* trigger_area, dt::Component* component) {
    if(trigger_area->getName() == "triggerArea" && component->getName() == "meshBody") {
        mAreaTriggered = true;
    }
}

void Main::onInitialize() {
    dt::Scene::SceneSP scene = addScene(new dt::Scene("testscene"));

    dt::ResourceManager::get()->addResourceLocation("sinbad.zip","Zip", true);
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

    dt::Node::NodeSP camnode = scene->addChildNode(new dt::Node("camnode"));
    camnode->setPosition(Ogre::Vector3(0, 0, 50));
    camnode->addComponent(new dt::CameraComponent("cam"))->lookAt(Ogre::Vector3(0, 0, 0));;

    dt::Node::NodeSP meshnode1 = _addMeshNode(scene.get(), "meshNode1", Ogre::Vector3(-15.0f, 20.0f, 0.0f));

    meshnode1->addComponent(new dt::PhysicsBodyComponent("meshNode1", "meshBody"));

    dt::Node::NodeSP triggerAreaNode = scene->addChildNode(new dt::Node("triggerArea"));
    std::shared_ptr<dt::TriggerAreaComponent> triggerAreaComponent = triggerAreaNode->addComponent(new dt::TriggerAreaComponent(new btBoxShape(btVector3(5.0f, 5.0f, 5.0f)), "triggerArea"));
    triggerAreaNode->setPosition(Ogre::Vector3(-15.0f, 0.0f, 0.0f));

    QObject::connect(triggerAreaComponent.get(), 
                     SIGNAL(Triggered(dt::TriggerAreaComponent*, dt::Component*)), 
                     this, 
                     SLOT(AreaTriggered(dt::TriggerAreaComponent*, 
                     dt::Component*)));

}

}
