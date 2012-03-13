
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_TESTS_PHYSICSSTRESSTEST
#define DUCTTAPE_ENGINE_TESTS_PHYSICSSTRESSTEST

#include <Config.hpp>

#include "Test.hpp"

#include <Core/Root.hpp>
#include <Graphics/LightComponent.hpp>
#include <Graphics/MeshComponent.hpp>
#include <Physics/PhysicsBodyComponent.hpp>
#include <Scene/Game.hpp>
#include <Scene/Node.hpp>
#include <Scene/Scene.hpp>
#include <Utils/Utils.hpp>

#include <OgreProcedural.h>

namespace PhysicsStressTest {

class PhysicsStressTest : public Test {
public:
    bool run(int argc, char** argv);
    QString getTestName();
};

////////////////////////////////////////////////////////////////

class Main : public dt::State {
    Q_OBJECT
public:
    Main();
    void onInitialize();
    void updateStateFrame(double simulation_frame_time);

private:
    double mRuntime;

};

} // namespace PhysicsStress

#endif
