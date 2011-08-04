
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

#include <QCoreApplication>
#include <QFile>
#include <QScriptEngine>

namespace dt {

/**
  * Manager class for the Scripting Engine.
  */
class DUCTTAPE_API ScriptManager : public Manager {
public:
    /**
      * Default constructor.
      */
    ScriptManager();

    void Initialize();
    void Deinitialize();
    static ScriptManager* Get();

    bool AddScript(QString script, QString name);
    bool LoadScript(QString path, QString name = "");

    bool ExecuteScript(QString name);
    QScriptValue GetLastReturnValue();

private:
    QScriptEngine* mScriptEngine;
    QMap<QString, QString> mScripts;
    QScriptValue mLastReturnValue;

};

}

#endif
