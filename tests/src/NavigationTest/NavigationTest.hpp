
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_TESTS_NAVIGATIONTEST
#define DUCTTAPE_ENGINE_TESTS_NAVIGATIONTEST

#include <Config.hpp>

#include "Test.hpp"

#include <Core/Root.hpp>
#include <Graphics/LightComponent.hpp>
#include <Graphics/MeshComponent.hpp>
#include <Logic/NavigationManager.hpp>
#include <Physics/PhysicsBodyComponent.hpp>
#include <Scene/Game.hpp>
#include <Scene/Node.hpp>
#include <Scene/Scene.hpp>

#include <OgreProcedural.h>

namespace NavigationTest {

class NavigationTest : public Test {
    
public:
    bool Run(int argc, char** argv);
    QString GetTestName();
};

////////////////////////////////////////////////////////////////

class Main : public dt::State {
    Q_OBJECT
public:
    Main();
    ~Main();
    //Main::Priority GetEventPriority() const;
    void OnInitialize();
    void UpdateStateFrame(double simulation_frame_time);

private:
    double mRuntime;
    Ogre::Vector3 mSphere1DisabledPosition;
    Ogre::Vector3 mSphere2DisabledPosition;
    dt::NavigationManager* mNavManager;

};

} // namespace NavigationTest

#endif
