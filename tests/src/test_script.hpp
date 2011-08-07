
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_TESTS_TEST_SCRIPT
#define DUCTTAPE_ENGINE_TESTS_TEST_SCRIPT

#include <Core/Root.hpp>
#include <Logic/ScriptManager.hpp>
#include <Utils/Logger.hpp>

#include <QCoreApplication>
#include <QString>

namespace test_script {

bool failed = false;

QScriptValue run(QString name, QScriptValue expected_value = QScriptValue::UndefinedValue) {
    std::cout << "================================================================" << std::endl;
    std::cout << "Running script: \"" << name.toStdString() << "\"..." << std::endl;

    QScriptValue val;
    bool this_failed = false;
    if(!dt::ScriptManager::Get()->ExecuteScript(name)) {
        std::cerr << "Execution of script \"" <<  name.toStdString() << "\" failed." << std::endl;
        this_failed = true;
    } else {
        val = dt::ScriptManager::Get()->GetLastReturnValue();
        if(val.isError() && !val.equals(expected_value)) { // error, and no error expected
            std::cerr << "ERROR: Script error." << std::endl;
            std::cerr << "Message:  " << val.toString().toStdString() << std::endl;
            this_failed = true;
        } else if(!expected_value.isUndefined() && !val.equals(expected_value)) {
            std::cerr << "ERROR: Wrong value." << std::endl;
            std::cerr << "Returned: " << val.toString().toStdString() << std::endl;
            std::cerr << "Expected: " << expected_value.toString().toStdString() << std::endl;
            this_failed = true;
        }
    }

    if(!this_failed) {
        std::cout << "Script test successful." << std::endl;
    } else {
        failed = true;
    }
    return val;
}

int Run(int argc, char** argv) {
    dt::Root::GetInstance().Initialize(argc, argv);

    dt::ScriptManager::Get()->AddScript("print(DT_VERSION);", "print_test");
    dt::ScriptManager::Get()->AddScript("DT_VERSION", "return_version");
    dt::ScriptManager::Get()->AddScript("print(TotalTime);", "update_context");
    dt::ScriptManager::Get()->LoadScript("scripts/test_load_script_file.js");

    run("print_test");
    run("return_version", dt::Root::_VERSION);
    run("test_load_script_file.js", "test");

    sf::Sleep(100);
    dt::ScriptManager::Get()->UpdateContext();
    run("update_context");

    if(failed) {
        std::cout << "Test Script: Errors occured." << std::endl;
        return 1;
    }

    std::cout << "Test Script: OK" << std::endl;
    dt::Root::GetInstance().Deinitialize();
    return 0;
}

} // namespace test_script

#endif
