
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include "MouseCursorTest/MouseCursorTest.hpp"

#include <Event/BeginFrameEvent.hpp>
#include <Scene/StateManager.hpp>
#include <Core/ResourceManager.hpp>
#include <Graphics/CameraComponent.hpp>
#include <Input/InputManager.hpp>

namespace MouseCursorTest {

bool MouseCursorTest::Run(int argc, char** argv) {
    dt::Game game;
    game.Run(new Main(), argc, argv);
    return true;
}

QString MouseCursorTest::GetTestName() {
    return "MouseCursor";
}

////////////////////////////////////////////////////////////////

Main::Main()
    : mRuntime(0) {}

void Main::HandleEvent(std::shared_ptr<dt::Event> e) {
    if(e->GetType() == "DT_BEGINFRAMEEVENT") {
        mRuntime += std::dynamic_pointer_cast<dt::BeginFrameEvent>(e)->GetFrameTime();

        if(mRuntime >= 0 && mStep == 0) {
            mStep = 1;
            dt::Logger::Get().Debug("Mode: Hidden");
            dt::InputManager::Get()->SetMouseCursorMode(dt::InputManager::HIDDEN);
        } else if(mRuntime >= 0.5 && mStep == 1) {
            mStep = 2;
            dt::Logger::Get().Debug("Mode: Graphical");
            dt::InputManager::Get()->SetMouseCursorMode(dt::InputManager::GRAPHICAL);
        } else if(mRuntime >= 1.0 && mStep == 2) {
            mStep = 3;
            dt::Logger::Get().Debug("Mode: System");
            dt::InputManager::Get()->SetMouseCursorMode(dt::InputManager::SYSTEM);
        } else if(mRuntime >= 1.5 && mStep == 3) {
            mStep = 4;
            dt::Logger::Get().Debug("Mode: Graphical again");
            dt::InputManager::Get()->SetMouseCursorMode(dt::InputManager::GRAPHICAL);
        } else if(mRuntime >= 2.0 && mStep == 4) {
           mStep = 5;
           dt::Logger::Get().Debug("Mode: Hidden again");
           dt::InputManager::Get()->SetMouseCursorMode(dt::InputManager::HIDDEN);
        }
        if(mRuntime > 2.5) {
            dt::StateManager::Get()->Pop(1);
        }
    }
}

void Main::OnInitialize() {
    mStep = 0;
    dt::ResourceManager::Get()->AddResourceLocation("gui","FileSystem", true);

    // need to create a camera so we have a window
    dt::Scene* scene = AddScene(new dt::Scene("testscene"));
    scene->AddChildNode(new dt::Node("camnode"))->AddComponent<dt::CameraComponent>(new dt::CameraComponent("cam"));
}

}
