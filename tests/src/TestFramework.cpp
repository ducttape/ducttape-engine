
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

#include <iostream>

void AddTest(Test* test) {
    QString name(test->GetTestName());
    Tests.insert(name, test);
}

Test* GetTest(QString name) {
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
    AddTest(new CamerasTest::CamerasTest);
    AddTest(new ConnectionsTest::ConnectionsTest);
    AddTest(new DisplayTest::DisplayTest);
    AddTest(new FollowPathTest::FollowPathTest);
    AddTest(new GuiTest::GuiTest);
    AddTest(new InputTest::InputTest);
    AddTest(new LoggerTest::LoggerTest);
    AddTest(new MouseCursorTest::MouseCursorTest);
    AddTest(new MusicFadeTest::MusicFadeTest);
    AddTest(new MusicTest::MusicTest);
    AddTest(new NamesTest::NamesTest);
    AddTest(new NetworkTest::NetworkTest);
    AddTest(new ParticlesTest::ParticlesTest);
    AddTest(new PhysicsSimpleTest::PhysicsSimpleTest);
    AddTest(new PhysicsStressTest::PhysicsStressTest);
    AddTest(new PrimitivesTest::PrimitivesTest);
    AddTest(new QObjectTest::QObjectTest);
    AddTest(new RandomTest::RandomTest);
    AddTest(new ResourceManagerTest::ResourceManagerTest);
    AddTest(new SerializationBinaryTest::SerializationBinaryTest);
    AddTest(new SerializationYamlTest::SerializationYamlTest);
    AddTest(new ScriptComponentTest::ScriptComponentTest);
    AddTest(new ScriptingTest::ScriptingTest);
    AddTest(new ShadowsTest::ShadowsTest);
    AddTest(new SignalsTest::SignalsTest);
    AddTest(new SoundTest::SoundTest);
    AddTest(new StatesTest::StatesTest);
    AddTest(new TextTest::TextTest);
    AddTest(new TimerTest::TimerTest);
    AddTest(new TerrainTest::TerrainTest);
    AddTest(new BillboardTest::BillboardTest);

    if(argc < 2) {
        std::cout << "TestFramework usage: " << std::endl;
        std::cout << "  ./TestFramework <test name>" << std::endl;
        std::cout << std::endl << "Available tests:" << std::endl;
        for(auto iter = Tests.begin(); iter != Tests.end(); ++iter) {
            std::cout << "  - " << dt::Utils::ToStdString(iter->first) << std::endl;
        }
    } else {
        bool failure = false;

        for(int i = 1; i < argc; ++i) {
            QString name(argv[i]);
            if(name == "client" || name == "server") // ignore parameters of network
                continue;
            std::cout << "Running test " + dt::Utils::ToStdString(name) + "..." << std::endl;
            Test* test = GetTest(name);
            if(test == nullptr) {
                std::cerr << "Test " + dt::Utils::ToStdString(name) + " not found. Skipping." << std::endl;
            } else if(!test->Run(argc, argv)) {
                failure = true;
                std::cerr << "Test " + dt::Utils::ToStdString(name) + " FAILED." << std::endl;
            } else {
                std::cout << "Test " + dt::Utils::ToStdString(name) + ": OK." << std::endl;
            }
        }

        if(failure) {
            std::cerr << std::endl << "Not all tests successful, check log for details." << std::endl;
            return 1;
        }
    }
    return 0;
}
