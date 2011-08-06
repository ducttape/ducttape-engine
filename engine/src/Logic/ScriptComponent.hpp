
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_LOGIC_SCRIPTCOMPONENT
#define DUCTTAPE_ENGINE_LOGIC_SCRIPTCOMPONENT

#include <Config.hpp>

#include <Scene/Component.hpp>

#include <QScriptValue>
#include <QString>

namespace dt {

/**
  * Loads a script, bound to a node.
  * @see Component
  */
class DUCTTAPE_API ScriptComponent : public Component {
    Q_OBJECT
public:
    /**
      * Advanced constructor.
      * @param name The name for this component.
      * @see Component
      */
    ScriptComponent(const QString& script_name, const QString& name = "");

    void OnCreate();
    void OnDestroy();
    void OnEnable();
    void OnDisable();
    void OnUpdate(double time_diff);

protected:
    QScriptValue _CallScriptFunction(QString name, QScriptValueList params = QScriptValueList());

    QString mScriptName;
    QScriptValue mScriptObject;
    bool mValid;
};

}

#endif
