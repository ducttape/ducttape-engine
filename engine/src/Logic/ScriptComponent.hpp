
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
    DT_SERIALIZABLE(ScriptComponent)
    /**
      * Advanced constructor.
      * @param script_name The name for the script.
      * @param name The name for this component.
      * @param is_update_enabled Whether update call of the script is enabled or not.
      * @see Component
      */
    ScriptComponent(const QString& script_name, const QString& name = "", bool is_update_enabled = false);

    void OnInitialize();
    void OnDeinitialize();
    void OnEnable();
    void OnDisable();
    void OnUpdate(double time_diff);

    /**
      * Gets if update call of the script is enabled or not.
      * @return Whether update call of the script is enabled or not.
      */
    bool IsUpdateEnabled() const;

    /**
      * Sets if update call of the script is enabled or not.
      * @param is_enabled Whether update call of the script is enabled or not.
      */
    void SetUpdateEnabled(bool is_enabled);

protected:
    QScriptValue _CallScriptFunction(QString name, QScriptValueList params = QScriptValueList());

    QString mScriptName;
    QScriptValue mScriptObject;
    bool mValid;
    bool mIsUpdateEnabled;
};

}

#endif
