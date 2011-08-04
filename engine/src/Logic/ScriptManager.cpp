
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include <Logic/ScriptManager.hpp>

#include <Core/Root.hpp>
#include <Utils/Logger.hpp>
#include <Utils/Utils.hpp>

#include <QTextStream>

namespace dt {

ScriptManager::ScriptManager() {}

void ScriptManager::Initialize() {
    mScriptEngine = new QScriptEngine();

    // setup globals and engine info
    mScriptEngine->globalObject().setProperty("DT_VERSION", DUCTTAPE_VERSION);
}

void ScriptManager::Deinitialize() {}

ScriptManager* ScriptManager::Get() {
    return Root::GetInstance().GetScriptManager();
}

bool ScriptManager::AddScript(QString script, QString name) {
    if(name == "") {
        Logger::Get().Error("Cannot add script without name.");
        return false;
    }
    if(mScripts.contains(name)) {
        Logger::Get().Error("Cannot add script \"" + name + "\": a script with this name already exists.");
        return false;
    }

    Logger::Get().Debug("Adding script \"" + name + "\".");
    mScripts[name] = script;
    return true;
}

bool ScriptManager::LoadScript(QString path, QString name) {
    QFileInfo&& info = ResourceManager::Get()->FindFile(path);
    QFile file(info.absoluteFilePath());

    if(!file.exists())
        return false;

    // check if file is open or open it
    if(!(file.isOpen() || file.open(QIODevice::ReadOnly | QIODevice::Text))) {
        Logger::Get().Error("Cannot open file <" + path + ">: " + file.errorString());
        return false;
    }

    QTextStream stream(&file);
    QString script = stream.readAll();
    file.close();

    if(name == "") {
        name = info.fileName();
    }

    return AddScript(script, name);
}

bool ScriptManager::ExecuteScript(QString name) {
    if(name == "" || !mScripts.contains(name)) {
        Logger::Get().Error("Cannot execute script \"" + name + "\": script not found.");
        return false;
    }

    // do the magix
    mLastReturnValue = mScriptEngine->evaluate(mScripts[name], name);

    if(mScriptEngine->hasUncaughtException()) {
        int line = mScriptEngine->uncaughtExceptionLineNumber();
        Logger::Get().Error( QString("SCRIPT EXCEPTION -- %1:%2 -- %3").arg(name, Utils::ToString(line), mLastReturnValue.toString()) );
        return false;
    } else {
        if(mLastReturnValue.isUndefined()) {
            Logger::Get().Debug("Script executed successfully and returned no value.");
        } else {
            Logger::Get().Debug("Script returned value: " + mLastReturnValue.toString());
        }
    }

    return true;
}

QScriptValue ScriptManager::GetLastReturnValue() {
    return mLastReturnValue;
}

}
