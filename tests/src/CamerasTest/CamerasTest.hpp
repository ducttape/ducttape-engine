
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_TESTS_CAMERASTEST
#define DUCTTAPE_ENGINE_TESTS_CAMERASTEST

#include <Config.hpp>

#include "Test.hpp"

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
    bool run(int argc, char** argv);
    QString getTestName();
};

class Main : public dt::State {
    Q_OBJECT
public:
    Main();
    void onInitialize();
    void updateStateFrame(double simulation_frame_time);

private:
    double mRuntime;
    int mStep;
    std::shared_ptr<dt::CameraComponent>  mCamera1;
    std::shared_ptr<dt::CameraComponent>  mCamera2;

};

} // namespace test_multiple_camera

#endif
