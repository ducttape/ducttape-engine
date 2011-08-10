#include "ScriptingTest/ScriptingTest.hpp"

namespace ScriptingTest {

bool ScriptingTest::Run(int argc, char** argv) {
    mFailed = false;

    dt::Root::GetInstance().Initialize(argc, argv);

    dt::ScriptManager::Get()->AddScript("print(DT_VERSION);", "print_test");
    dt::ScriptManager::Get()->AddScript("DT_VERSION", "return_version");
    dt::ScriptManager::Get()->AddScript("print(TotalTime);", "update_context");
    dt::ScriptManager::Get()->LoadScript("scripts/test_load_script_file.js");

    RunScript("print_test");
    RunScript("return_version", dt::Root::_VERSION);
    RunScript("test_load_script_file.js", "test");

    sf::Sleep(100);
    dt::ScriptManager::Get()->UpdateContext();
    RunScript("update_context");

    if(mFailed) {
        std::cout << "Test Script: Errors occured." << std::endl;
        return false;
    }

    dt::Root::GetInstance().Deinitialize();
    return true;
}

QString ScriptingTest::GetTestName() {
    return "Scripting";
}

QScriptValue ScriptingTest::RunScript(QString name, QScriptValue expected_value) {
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
        mFailed = true;
    }
    return val;
}

}
