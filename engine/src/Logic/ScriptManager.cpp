
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include <Logic/ScriptManager.hpp>

#include <Core/Root.hpp>
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

void ScriptManager::initialize() {
    mScriptEngine = new QScriptEngine();

    // setup globals and engine info
    mGlobalObject = mScriptEngine->globalObject();
    mGlobalObject.setProperty("DT_VERSION", DUCTTAPE_VERSION);
    // redirect print output
    mGlobalObject.setProperty("print", mScriptEngine->newFunction(&ScriptManager::scriptPrintFunction));

    QScriptValue display_manager = mScriptEngine->newQObject(DisplayManager::get());
    mScriptEngine->globalObject().setProperty("DisplayManager", display_manager);

    QScriptValue gui_root = mScriptEngine->newQObject(& GuiManager::get()->getRootWindow());
    mScriptEngine->globalObject().setProperty("Gui", gui_root);

    QScriptValue keyboard = mScriptEngine->newQObject(new KeyboardState());
    mScriptEngine->globalObject().setProperty("Keyboard", keyboard);

    QScriptValue mouse = mScriptEngine->newQObject(new MouseState());
    mScriptEngine->globalObject().setProperty("Mouse", mouse);
}

void ScriptManager::deinitialize() {}

ScriptManager* ScriptManager::get() {
    return Root::getInstance().getScriptManager();
}

bool ScriptManager::addScript(QString script, QString name) {
    if(name == "") {
        Logger::get().error("Cannot add script without name.");
        return false;
    }

    if(hasScript(name)) {
        Logger::get().error("Cannot add script \"" + name + "\": a script with this name already exists.");
        return false;
    }

    // check the syntax
    QScriptSyntaxCheckResult syntax = mScriptEngine->checkSyntax(script);
    if(syntax.state() != QScriptSyntaxCheckResult::Valid) {
        Logger::get().error("Syntax error in script \"" + name + "\" at line "
                            + Utils::toString(syntax.errorLineNumber()) + " column "
                            + Utils::toString(syntax.errorColumnNumber()) + ":");
        Logger::get().error("    " +  syntax.errorMessage());
    } else {
        Logger::get().debug("Adding script \"" + name + "\".");
        mScripts[name] = QScriptProgram(script, name);
    }
    return true;
}

bool ScriptManager::loadScript(QString path, QString name) {
    QFileInfo&& info = ResourceManager::get()->findFile(path);
    QFile file(info.absoluteFilePath());

    if(!file.exists())
        return false;

    // check if file is open or open it
    if(!(file.isOpen() || file.open(QIODevice::ReadOnly | QIODevice::Text))) {
        Logger::get().error("Cannot open file <" + path + ">: " + file.errorString());
        return false;
    }

    QTextStream stream(&file);
    QString script = stream.readAll();
    file.close();

    if(name == "") {
        name = info.fileName();
    }

    return addScript(script, name);
}

bool ScriptManager::hasScript(QString name) {
    return mScripts.contains(name);
}

void ScriptManager::updateContext(QScriptEngine* engine) {
    if(engine == nullptr) {
        engine = mScriptEngine;
    }

    engine->globalObject().setProperty("TotalTime", Root::getInstance().getTimeSinceInitialize());
}

void ScriptManager::updateContext(QScriptValue object) {
    updateContext(object.engine());
}

bool ScriptManager::executeScript(QString name) {
    if(name == "" || !hasScript(name)) {
        Logger::get().error("Cannot execute script \"" + name + "\": script not found.");
        return false;
    }

    return _evaluate(mScripts[name]);
}

bool ScriptManager::evaluate(QString snippet, QString context) {
    return _evaluate(QScriptProgram(snippet, context));
}

QScriptValue ScriptManager::getLastReturnValue() {
    return mLastReturnValue;
}

QScriptValue ScriptManager::getScriptObject(QString name, ScriptComponent* component) {
    if(!hasScript(name)) {
        return QScriptValue::UndefinedValue;
    }

    // save the global object and set an empty one
    QScriptValue global(mScriptEngine->globalObject());
    mScriptEngine->setGlobalObject(mScriptEngine->newObject());

    // write script members/functions as properties into global object
    // like this we have the same engine linked, so the
    executeScript(name);

    // extract the global object and reset the original one
    QScriptValue obj(mScriptEngine->globalObject());
    mScriptEngine->setGlobalObject(global);

    // Set the object's component member.
    QScriptValue componentObject = mScriptEngine->newQObject(component);
    QScriptValue prop = obj.property("component");
    if(prop.isValid()) {
        Logger::get().warning("Overriding member \"component\" in script \"" + name + "\" with ScriptComponent \"" + component->getName() + "\".");
        Logger::get().info(" > Previous Value: " + prop.toString());
        Logger::get().info(" > Previous  Type: " + QString(prop.toVariant().typeName()) );
    }
    obj.setProperty("component", componentObject);

    return obj;
}

QScriptEngine* ScriptManager::getScriptEngine() {
    return mScriptEngine;
}

bool ScriptManager::handleErrors(QString context) {
    if(mScriptEngine->hasUncaughtException()) {
        int line = mScriptEngine->uncaughtExceptionLineNumber();
        Logger::get().error( QString("SCRIPT EXCEPTION -- %1:%2 -- %3").arg(context,
                Utils::toString(line), mLastReturnValue.toString()) );
        return false;
    }

    return true;
}

QScriptValue ScriptManager::scriptPrintFunction(QScriptContext* context, QScriptEngine* engine) {
    QString line;
    for(int i = 0; i < context->argumentCount(); ++i) {
        // append the argument, converted to string
        line.append(context->argument(i).toString());
    }
    Logger::get().info("Script output: " + line);
    return engine->undefinedValue();
}

bool ScriptManager::_evaluate(QScriptProgram program) {
    mLastReturnValue = mScriptEngine->evaluate(program);
    return handleErrors(program.fileName());
}

}
