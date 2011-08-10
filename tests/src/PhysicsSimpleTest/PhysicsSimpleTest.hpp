
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_TESTS_PHYSICSSIMPLETEST
#define DUCTTAPE_ENGINE_TESTS_PHYSICSSIMPLETEST

#include "Test.hpp"

#include <Core/Root.hpp>
#include <Event/EventListener.hpp>
#include <Graphics/LightComponent.hpp>
#include <Graphics/MeshComponent.hpp>
#include <Physics/PhysicsBodyComponent.hpp>
#include <Scene/Game.hpp>
#include <Scene/Node.hpp>
#include <Scene/Scene.hpp>

#include <OgreProcedural.h>

namespace PhysicsSimpleTest {

class PhysicsSimpleTest : public Test {
public:
    bool Run(int argc, char** argv);
    QString GetTestName();
};

////////////////////////////////////////////////////////////////

class Main : public dt::State {
public:
    Main();
    Main::Priority GetEventPriority() const;
    void HandleEvent(std::shared_ptr<dt::Event> e);
    void OnInitialize();
private:
    double mRuntime;
    Ogre::Vector3 mSphere1DisabledPosition;
    Ogre::Vector3 mSphere2DisabledPosition;

};

} // namespace PhysicsSimpleTest

#endif
