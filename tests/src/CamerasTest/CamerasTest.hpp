
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_TESTS_CAMERASTEST
#define DUCTTAPE_ENGINE_TESTS_CAMERASTEST

#include "Test.hpp"

#include <Event/BeginFrameEvent.hpp>
#include <Graphics/CameraComponent.hpp>
#include <Graphics/LightComponent.hpp>
#include <Graphics/MeshComponent.hpp>
#include <Scene/Game.hpp>
#include <Scene/Node.hpp>
#include <Scene/State.hpp>
#include <Scene/StateManager.hpp>
#include <Core/ResourceManager.hpp>

namespace CamerasTest {

class CamerasTest : public Test {
public:
    bool Run(int argc, char** argv);
    QString GetTestName();
};

class Main : public dt::State {
public:
    Main();
    void HandleEvent(std::shared_ptr<dt::Event> e);
    void OnInitialize();

private:
    double mRuntime;
    int mStep;
    dt::CameraComponent* mCamera1;
    dt::CameraComponent* mCamera2;

};

} // namespace test_multiple_camera

#endif
