
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_TESTS_SCRIPTINGTEST
#define DUCTTAPE_ENGINE_TESTS_SCRIPTINGTEST

#include <Config.hpp>

#include "Test.hpp"

#include <Core/Root.hpp>
#include <Logic/ScriptManager.hpp>

#include <QCoreApplication>
#include <QString>

namespace ScriptingTest {

class ScriptingTest : public Test {
public:
    bool run(int argc, char** argv);
    QString getTestName();
    QScriptValue runScript(QString name, QScriptValue expected_value = QScriptValue::UndefinedValue);
private:
    bool mFailed;
};

} // namespace ScriptingTest

#endif
