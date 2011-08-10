
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_TESTS_STATESTEST
#define DUCTTAPE_ENGINE_TESTS_STATESTEST

#include "Test.hpp"

#include <Core/Root.hpp>
#include <Event/EventListener.hpp>
#include <Graphics/CameraComponent.hpp>
#include <Graphics/LightComponent.hpp>
#include <Graphics/MeshComponent.hpp>
#include <Graphics/TextComponent.hpp>
#include <Scene/Game.hpp>
#include <Scene/Node.hpp>
#include <Scene/Scene.hpp>
#include <Graphics/TextComponent.hpp>

#include <OgreFontManager.h>

namespace StatesTest {

class StatesTest : public Test {
public:
    bool Run(int argc, char** argv);
    QString GetTestName();
};

////////////////////////////////////////////////////////////////

class SecondState : public dt::State {
public:
    void HandleEvent(std::shared_ptr<dt::Event> e);
    void OnInitialize();
private:
    bool mPopped;
};

////////////////////////////////////////////////////////////////

class FirstState : public dt::State {
public:
    FirstState();
    void HandleEvent(std::shared_ptr<dt::Event> e);
    void OnInitialize();
private:
    bool mCreated;
};

} // namespace test_physics

#endif
