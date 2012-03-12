
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include "TestFramework.hpp"

#include "CamerasTest/CamerasTest.hpp"
#include "ConnectionsTest/ConnectionsTest.hpp"
#include "DisplayTest/DisplayTest.hpp"
#include "FollowPathTest/FollowPathTest.hpp"
#include "GuiTest/GuiTest.hpp"
#include "InputTest/InputTest.hpp"
#include "LoggerTest/LoggerTest.hpp"
#include "MouseCursorTest/MouseCursorTest.hpp"
#include "MusicFadeTest/MusicFadeTest.hpp"
#include "MusicTest/MusicTest.hpp"
#include "NamesTest/NamesTest.hpp"
#include "NetworkTest/NetworkTest.hpp"
#include "ParticlesTest/ParticlesTest.hpp"
#include "PhysicsSimpleTest/PhysicsSimpleTest.hpp"
#include "PhysicsStressTest/PhysicsStressTest.hpp"
#include "PrimitivesTest/PrimitivesTest.hpp"
#include "QObjectTest/QObjectTest.hpp"
#include "RandomTest/RandomTest.hpp"
#include "ResourceManagerTest/ResourceManagerTest.hpp"
#include "SerializationBinaryTest/SerializationBinaryTest.hpp"
#include "SerializationYamlTest/SerializationYamlTest.hpp"
#include "ScriptComponentTest/ScriptComponentTest.hpp"
#include "ScriptingTest/ScriptingTest.hpp"
#include "ShadowsTest/ShadowsTest.hpp"
#include "SignalsTest/SignalsTest.hpp"
#include "SoundTest/SoundTest.hpp"
#include "StatesTest/StatesTest.hpp"
#include "TextTest/TextTest.hpp"
#include "TimerTest/TimerTest.hpp"
#include "TerrainTest/TerrainTest.hpp"
#include "Utils/Utils.hpp"
#include "BillboardTest/BillboardTest.hpp"
#include "GuiStateTest/GuiStateTest.hpp"

#include <iostream>

void addTest(Test* test) {
    QString name(test->getTestName());
    Tests.insert(name, test);
}

Test* getTest(QString name) {
    QString n(name.toLower());
    for(auto iter = Tests.begin(); iter != Tests.end(); ++iter) {
        if(iter->first.toLower() == n) {
            return iter->second;
        }
    }
    return nullptr;
}

int main(int argc, char** argv) {

    // add all tests
    addTest(new CamerasTest::CamerasTest);
    addTest(new ConnectionsTest::ConnectionsTest);
    addTest(new DisplayTest::DisplayTest);
    addTest(new FollowPathTest::FollowPathTest);
    addTest(new GuiTest::GuiTest);
    addTest(new InputTest::InputTest);
    addTest(new LoggerTest::LoggerTest);
    addTest(new MouseCursorTest::MouseCursorTest);
    addTest(new MusicFadeTest::MusicFadeTest);
    addTest(new MusicTest::MusicTest);
    addTest(new NamesTest::NamesTest);
    addTest(new NetworkTest::NetworkTest);
    addTest(new ParticlesTest::ParticlesTest);
    addTest(new PhysicsSimpleTest::PhysicsSimpleTest);
    addTest(new PhysicsStressTest::PhysicsStressTest);
    addTest(new PrimitivesTest::PrimitivesTest);
    addTest(new QObjectTest::QObjectTest);
    addTest(new RandomTest::RandomTest);
    addTest(new ResourceManagerTest::ResourceManagerTest);
    addTest(new SerializationBinaryTest::SerializationBinaryTest);
    addTest(new SerializationYamlTest::SerializationYamlTest);
    addTest(new ScriptComponentTest::ScriptComponentTest);
    addTest(new ScriptingTest::ScriptingTest);
    addTest(new ShadowsTest::ShadowsTest);
    addTest(new SignalsTest::SignalsTest);
    addTest(new SoundTest::SoundTest);
    addTest(new StatesTest::StatesTest);
    addTest(new TextTest::TextTest);
    addTest(new TimerTest::TimerTest);
    addTest(new TerrainTest::TerrainTest);
    addTest(new BillboardTest::BillboardTest);
    addTest(new GuiStateTest::GuiStateTest);

    if(argc < 2) {
        std::cout << "TestFramework usage: " << std::endl;
        std::cout << "  ./TestFramework <test name>" << std::endl;
        std::cout << std::endl << "Available tests:" << std::endl;
        for(auto iter = Tests.begin(); iter != Tests.end(); ++iter) {
            std::cout << "  - " << dt::Utils::toStdString(iter->first) << std::endl;
        }
    } else {
        bool failure = false;

        for(int i = 1; i < argc; ++i) {
            QString name(argv[i]);
            if(name == "client" || name == "server") // ignore parameters of network
                continue;
            std::cout << "Running test " + dt::Utils::toStdString(name) + "..." << std::endl;
            Test* test = getTest(name);
            if(test == nullptr) {
                std::cerr << "Test " + dt::Utils::toStdString(name) + " not found. Skipping." << std::endl;
            } else if(!test->run(argc, argv)) {
                failure = true;
                std::cerr << "Test " + dt::Utils::toStdString(name) + " FAILED." << std::endl;
            } else {
                std::cout << "Test " + dt::Utils::toStdString(name) + ": OK." << std::endl;
            }
        }

        if(failure) {
            std::cerr << std::endl << "Not all tests successful, check log for details." << std::endl;
            return 1;
        }
    }
    return 0;
}
