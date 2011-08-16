
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include <Logic/ScriptManager.hpp>

#include <Core/Root.hpp>
#include <Event/EventManager.hpp>
#include <Gui/GuiManager.hpp>
#include <Utils/Logger.hpp>
#include <Utils/Utils.hpp>
#include <Input/MouseState.hpp>
#include <Input/KeyboardState.hpp>
#include <Graphics/DisplayManager.hpp>
#include <Core/ResourceManager.hpp>

#include <QTextStream>

namespace dt {

ScriptManager::ScriptManager() {}

void ScriptManager::Initialize() {
    mScriptEngine = new QScriptEngine();

    // setup globals and engine info
    mGlobalObject = mScriptEngine->globalObject();
    mGlobalObject.setProperty("DT_VERSION", DUCTTAPE_VERSION);
    // redirect print output
    mGlobalObject.setProperty("print", mScriptEngine->newFunction(&ScriptManager::ScriptPrintFunction));

    QScriptValue display_manager = mScriptEngine->newQObject(DisplayManager::Get());
    mScriptEngine->globalObject().setProperty("DisplayManager", display_manager);

    QScriptValue gui_root = mScriptEngine->newQObject(& GuiManager::Get()->GetRootWindow());
    mScriptEngine->globalObject().setProperty("Gui", gui_root);

    QScriptValue keyboard = mScriptEngine->newQObject(new KeyboardState());
    mScriptEngine->globalObject().setProperty("Keyboard", keyboard);

    QScriptValue mouse = mScriptEngine->newQObject(new MouseState());
    mScriptEngine->globalObject().setProperty("Mouse", mouse);
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

    if(HasScript(name)) {
        Logger::Get().Error("Cannot add script \"" + name + "\": a script with this name already exists.");
        return false;
    }

    // check the syntax
    QScriptSyntaxCheckResult syntax = mScriptEngine->checkSyntax(script);
    if(syntax.state() != QScriptSyntaxCheckResult::Valid) {
        Logger::Get().Error("Syntax error in script \"" + name + "\" at line "
                            + Utils::ToString(syntax.errorLineNumber()) + " column "
                            + Utils::ToString(syntax.errorColumnNumber()) + ":");
        Logger::Get().Error("    " +  syntax.errorMessage());
    } else {
        Logger::Get().Debug("Adding script \"" + name + "\".");
        mScripts[name] = QScriptProgram(script, name);
    }
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

bool ScriptManager::HasScript(QString name) {
    return mScripts.contains(name);
}

void ScriptManager::UpdateContext(QScriptEngine* engine) {
    if(engine == nullptr) {
        engine = mScriptEngine;
    }

    engine->globalObject().setProperty("TotalTime", Root::GetInstance().GetTimeSinceInitialize());
}

void ScriptManager::UpdateContext(QScriptValue object) {
    UpdateContext(object.engine());
}

bool ScriptManager::ExecuteScript(QString name) {
    if(name == "" || !HasScript(name)) {
        Logger::Get().Error("Cannot execute script \"" + name + "\": script not found.");
        return false;
    }

    return _Evaluate(mScripts[name]);
}

bool ScriptManager::Evaluate(QString snippet, QString context) {
    return _Evaluate(QScriptProgram(snippet, context));
}

QScriptValue ScriptManager::GetLastReturnValue() {
    return mLastReturnValue;
}

QScriptValue ScriptManager::GetScriptObject(QString name, ScriptComponent* component) {
    if(!HasScript(name)) {
        return QScriptValue::UndefinedValue;
    }

    // save the global object and set an empty one
    QScriptValue global(mScriptEngine->globalObject());
    mScriptEngine->setGlobalObject(mScriptEngine->newObject());

    // write script members/functions as properties into global object
    // like this we have the same engine linked, so the
    ExecuteScript(name);

    // extract the global object and reset the original one
    QScriptValue obj(mScriptEngine->globalObject());
    mScriptEngine->setGlobalObject(global);

    // Set the object's component member.
    QScriptValue componentObject = mScriptEngine->newQObject(component);
    QScriptValue prop = obj.property("component");
    if(prop.isValid()) {
        Logger::Get().Warning("Overriding member \"component\" in script \"" + name + "\" with ScriptComponent \"" + component->GetName() + "\".");
        Logger::Get().Info(" > Previous Value: " + prop.toString());
        Logger::Get().Info(" > Previous  Type: " + QString(prop.toVariant().typeName()) );
    }
    obj.setProperty("component", componentObject);

    return obj;
}

QScriptEngine* ScriptManager::GetScriptEngine() {
    return mScriptEngine;
}

bool ScriptManager::HandleErrors(QString context) {
    if(mScriptEngine->hasUncaughtException()) {
        int line = mScriptEngine->uncaughtExceptionLineNumber();
        Logger::Get().Error( QString("SCRIPT EXCEPTION -- %1:%2 -- %3").arg(context,
                Utils::ToString(line), mLastReturnValue.toString()) );
        return false;
    }

    return true;
}

QScriptValue ScriptManager::ScriptPrintFunction(QScriptContext* context, QScriptEngine* engine) {
    QString line;
    for(int i = 0; i < context->argumentCount(); ++i) {
        // append the argument, converted to string
        line.append(context->argument(i).toString());
    }
    Logger::Get().Info("Script output: " + line);
    return engine->undefinedValue();
}

bool ScriptManager::_Evaluate(QScriptProgram program) {
    mLastReturnValue = mScriptEngine->evaluate(program);
    return HandleErrors(program.fileName());
}

}
