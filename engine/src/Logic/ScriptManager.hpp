
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_LOGIC_SCRIPTMANAGER
#define DUCTTAPE_ENGINE_LOGIC_SCRIPTMANAGER

#include <Config.hpp>

#include <Core/Manager.hpp>
#include <Logic/ScriptComponent.hpp>

#include <QCoreApplication>
#include <QFile>
#include <QScriptEngine>
#include <QScriptProgram>

namespace dt {

/**
  * Manager class for the Scripting Engine.
  */
class DUCTTAPE_API ScriptManager : public Manager {
    Q_OBJECT
public:
    /**
      * Default constructor.
      */
    ScriptManager();

    void initialize();
    void deinitialize();
    static ScriptManager* get();

    bool addScript(QString script, QString name);
    bool loadScript(QString path, QString name = "");
    bool hasScript(QString name);

    void updateContext(QScriptEngine* engine = nullptr);
    void updateContext(QScriptValue object);
    bool executeScript(QString name);
    bool evaluate(QString snippet, QString context = "unknown");
    QScriptValue getLastReturnValue();
    QScriptValue getScriptObject(QString name, ScriptComponent* component);
    QScriptEngine* getScriptEngine();

    bool handleErrors(QString context = "unknown");

    static QScriptValue scriptPrintFunction(QScriptContext* context, QScriptEngine* engine);

private:
    bool _evaluate(QScriptProgram program);
    QScriptEngine* mScriptEngine;
    QMap<QString, QScriptProgram> mScripts;
    QScriptValue mLastReturnValue;
    QScriptValue mGlobalObject;

};

}

#endif
