
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_TESTS_TRIGGERAREACOMPONENTTEST
#define DUCTTAPE_ENGINE_TESTS_TRIGGERAREACOMPONENTTEST

#include <Config.hpp>

#include "Test.hpp"

#include <Core/Root.hpp>
#include <Graphics/MeshComponent.hpp>
#include <Logic/TriggerAreaComponent.hpp>
#include <Scene/Node.hpp>
#include <Scene/Scene.hpp>
#include <Scene/StateManager.hpp>


namespace TriggerAreaComponentTest {

class TriggerAreaComponentTest : public Test {
public:
    bool Run(int argc, char** argv);
    QString GetTestName();
};


class Main: public dt::State {
    Q_OBJECT
public:
    Main();
    void OnInitialize();
    void UpdateStateFrame(double simulation_frame_time);

 public slots:
     void AreaTriggered(dt::TriggerAreaComponent* trigger_area, dt::Component* component);

private:
    dt::Node* _AddMeshNode(dt::Scene* scene, std::string name, Ogre::Vector3 pos);

    double mRuntime;
    bool mAreaTriggered;
};

}

#endif