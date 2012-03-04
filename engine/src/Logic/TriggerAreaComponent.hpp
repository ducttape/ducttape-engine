
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_LOGIC_TRIGGER_AREA_COMPONENT
#define DUCTTAPE_ENGINE_LOGIC_TRIGGER_AREA_COMPONENT

#include <Config.hpp>

#include <Scene/Component.hpp>
#include <Logic/ScriptComponent.hpp>
#include <Scene/Node.hpp>
#include <OgreAxisAlignedBox.h>

namespace dt
{

class DUCTTAPE_API TriggerAreaComponent : public Component
{
    Q_OBJECT
public:
    /**
    * Constructor.
    * @param script Script to trigger
    * @param node Node that will trigger the script when moved into area
    * @param area Area, entering which will trigger the script (in node space)
    * @param name The name of the component.
    */
    TriggerAreaComponent(ScriptComponent* script, 
                         Node* node, 
                         Ogre::AxisAlignedBox area, 
                         const QString& name = "");

    void OnCreate();
    void OnDestroy();
    void OnEnable();
    void OnDisable();
    void OnUpdate(double time_diff);

    /**
    * Setter for a flag indicating if the script should 
    * be switched off when the node leaves the area
    */
    void SetOneShotMode(bool oneShotMode);
    void SetNode(Node* node);
    void SetScript(ScriptComponent* script);

private:
    ScriptComponent* mScript;
    Node* mTriggeringNode;
    Ogre::AxisAlignedBox mArea;
    bool mOneShotMode;

};

}

#endif