
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_TESTS_BILLBOARDTEST
#define DUCTTAPE_ENGINE_TESTS_BILLBOARDTEST

#include <Config.hpp>

#include "Test.hpp"

#include <Core/Root.hpp>
#include <Graphics/CameraComponent.hpp>
#include <Graphics/BillboardSetComponent.hpp>
#include <Graphics/MeshComponent.hpp>
#include <Graphics/LightComponent.hpp>
#include <Logic/FollowPathComponent.hpp>
#include <Scene/Game.hpp>
#include <Scene/Node.hpp>
#include <Scene/Scene.hpp>
#include <Scene/StateManager.hpp>
#include <Core/ResourceManager.hpp>

#include <OgreProcedural.h>

namespace BillboardTest {

class BillboardTest : public Test {
public:
    bool Run(int argc, char** argv);
    QString GetTestName();
};


class Main : public dt::State {
public:
    Main();
    void OnInitialize();
    void UpdateStateFrame(double simulation_frame_time);

private:
    double mRuntime;
    dt::Node* mCamNode;

};

} // namespace test_billboard

#endif
