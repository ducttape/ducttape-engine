
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include <Core/Root.hpp>
#include <Logic/ScriptManager.hpp>
#include <Utils/Logger.hpp>

#include <QCoreApplication>
#include <QString>

bool failed = false;

void run(QString name, QScriptValue expected_value = QScriptValue::UndefinedValue) {
    if(!dt::ScriptManager::Get()->ExecuteScript(name)) {
        std::cerr << "Execution of script \"" <<  name.toStdString() << "\" failed." << std::endl;
        failed = true;
    }

    QScriptValue val = dt::ScriptManager::Get()->GetLastReturnValue();
    if(!val.equals(expected_value)) {
        std::cerr << "Script \"" <<  name.toStdString() << "\" returned wrong value." << std::endl;
        std::cerr << "=> \"" << val.toString().toStdString() << "\" instead of \"" << expected_value.toString().toStdString() << "\"." << std::endl;
        failed = true;
    }
    std::cout << "Script \"" << name.toStdString() << "\" test successful." << std::endl;
}

int main(int argc, char** argv) {
    dt::Root::GetInstance().Initialize(argc, argv);

    dt::ScriptManager::Get()->AddScript("print(DT_VERSION);", "print_test");
    dt::ScriptManager::Get()->AddScript("DT_VERSION", "return_version");
    dt::ScriptManager::Get()->LoadScript("scripts/test_load_script_file.js");

    run("print_test");
    run("return_version", dt::Root::_VERSION);
    run("test_load_script_file.js", "test");

    if(failed) {
        std::cout << "Test Script: Errors occured." << std::endl;
        return 1;
    }

    std::cout << "Test Script: OK" << std::endl;
    dt::Root::GetInstance().Deinitialize();
    return 0;
}
