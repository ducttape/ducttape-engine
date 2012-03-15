
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include "MouseCursorTest/MouseCursorTest.hpp"

//#include <Event/BeginFrameEvent.hpp>
#include <Scene/StateManager.hpp>
#include <Core/ResourceManager.hpp>
#include <Graphics/CameraComponent.hpp>
#include <Input/InputManager.hpp>

namespace MouseCursorTest {

bool MouseCursorTest::run(int argc, char** argv) {
    dt::Game game;
    game.run(new Main(), argc, argv);
    return true;
}

QString MouseCursorTest::getTestName() {
    return "MouseCursor";
}

////////////////////////////////////////////////////////////////

Main::Main()
    : mRuntime(0) {}

void Main::updateStateFrame(double simulation_frame_time) {
    mRuntime += simulation_frame_time;

    if(mRuntime >= 0 && mStep == 0) {
        mStep = 1;
        dt::Logger::get().debug("Mode: Hidden");
        dt::InputManager::get()->setMouseCursorMode(dt::InputManager::HIDDEN);
    } else if(mRuntime >= 0.5 && mStep == 1) {
        mStep = 2;
        dt::Logger::get().debug("Mode: Graphical");
        dt::InputManager::get()->setMouseCursorMode(dt::InputManager::GRAPHICAL);
    } else if(mRuntime >= 1.0 && mStep == 2) {
        mStep = 3;
        dt::Logger::get().debug("Mode: System");
        dt::InputManager::get()->setMouseCursorMode(dt::InputManager::SYSTEM);
    } else if(mRuntime >= 1.5 && mStep == 3) {
        mStep = 4;
        dt::Logger::get().debug("Mode: Graphical again");
        dt::InputManager::get()->setMouseCursorMode(dt::InputManager::GRAPHICAL);
    } else if(mRuntime >= 2.0 && mStep == 4) {
        mStep = 5;
        dt::Logger::get().debug("Mode: Hidden again");
        dt::InputManager::get()->setMouseCursorMode(dt::InputManager::HIDDEN);
    }
    if(mRuntime > 2.5) {
        dt::StateManager::get()->pop(1);
    }
}

void Main::onInitialize() {
    mStep = 0;
    dt::ResourceManager::get()->addResourceLocation("gui","FileSystem", true);

    // need to create a camera so we have a window
    dt::Scene::SceneSP scene = addScene(new dt::Scene("testscene"));
    scene->addChildNode(new dt::Node("camnode"))->addComponent<dt::CameraComponent>(new dt::CameraComponent("cam"));
}

}
