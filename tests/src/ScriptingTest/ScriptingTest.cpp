
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include "ScriptingTest/ScriptingTest.hpp"

#include <Utils/Utils.hpp>

#include <SFML/System.hpp>

#include <iostream>

namespace ScriptingTest {

bool ScriptingTest::run(int argc, char** argv) {
    mFailed = false;

    dt::Root::getInstance().initialize(argc, argv);

    dt::ScriptManager::get()->addScript("print(DT_VERSION);", "print_test");
    dt::ScriptManager::get()->addScript("DT_VERSION", "return_version");
    dt::ScriptManager::get()->addScript("print(TotalTime);", "update_context");
    dt::ScriptManager::get()->loadScript("scripts/test_load_script_file.js");

    runScript("print_test");
    runScript("return_version", dt::Root::_VERSION);
    runScript("test_load_script_file.js", "test");

    sf::sleep(sf::milliseconds(100));
    dt::ScriptManager::get()->updateContext();
    runScript("update_context");

    if(mFailed) {
        std::cout << "Test Script: Errors occured." << std::endl;
        return false;
    }

    dt::Root::getInstance().deinitialize();
    return true;
}

QString ScriptingTest::getTestName() {
    return "Scripting";
}

QScriptValue ScriptingTest::runScript(QString name, QScriptValue expected_value) {
    std::cout << "================================================================" << std::endl;
    std::cout << "Running script: \"" << dt::Utils::toStdString(name) << "\"..." << std::endl;

    QScriptValue val;
    bool this_failed = false;
    if(!dt::ScriptManager::get()->executeScript(name)) {
        std::cerr << "Execution of script \"" <<  dt::Utils::toStdString(name) << "\" failed." << std::endl;
        this_failed = true;
    } else {
        val = dt::ScriptManager::get()->getLastReturnValue();
        if(val.isError() && !val.equals(expected_value)) { // error, and no error expected
            std::cerr << "ERROR: Script error." << std::endl;
            std::cerr << "Message:  " << dt::Utils::toStdString(val.toString()) << std::endl;
            this_failed = true;
        } else if(!expected_value.isUndefined() && !val.equals(expected_value)) {
            std::cerr << "ERROR: Wrong value." << std::endl;
            std::cerr << "Returned: " << dt::Utils::toStdString(val.toString()) << std::endl;
            std::cerr << "Expected: " << dt::Utils::toStdString(expected_value.toString()) << std::endl;
            this_failed = true;
        }
    }

    if(!this_failed) {
        std::cout << "Script test successful." << std::endl;
    } else {
        mFailed = true;
    }
    return val;
}

}
