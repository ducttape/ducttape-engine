
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_TESTS_PHYSICSSTRESSTEST
#define DUCTTAPE_ENGINE_TESTS_PHYSICSSTRESSTEST

#include "Test.hpp"

#include <Core/Root.hpp>
#include <Event/EventListener.hpp>
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
    bool Run(int argc, char** argv);
    QString GetTestName();
};

////////////////////////////////////////////////////////////////

class Main : public dt::State {
public:
    Main();
    void HandleEvent(std::shared_ptr<dt::Event> e);
    void OnInitialize();
private:
    double mRuntime;

};

} // namespace PhysicsStress

#endif
