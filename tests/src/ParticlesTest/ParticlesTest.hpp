
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_TESTS_PARTICLESTEST
#define DUCTTAPE_ENGINE_TESTS_PARTICLESTEST

#include "Test.hpp"

#include <Core/Root.hpp>
#include <Event/EventListener.hpp>
#include <Graphics/MeshComponent.hpp>
#include <Graphics/ParticleSystemComponent.hpp>
#include <Logic/FollowPathComponent.hpp>
#include <Scene/Game.hpp>
#include <Scene/Node.hpp>
#include <Scene/Scene.hpp>

namespace ParticlesTest {

class ParticlesTest : public Test {
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

} // namespace test_particles

#endif
