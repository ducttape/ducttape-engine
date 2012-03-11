
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_LOGIC_TRIGGER_AREA_COMPONENT
#define DUCTTAPE_ENGINE_LOGIC_TRIGGER_AREA_COMPONENT

#include <memory>

#include <Config.hpp>

#include <Scene/Component.hpp>
#include <Logic/ScriptComponent.hpp>

#include <btBulletCollisionCommon.h>
#include <BulletCollision\CollisionDispatch\btGhostObject.h>

namespace dt {

class DUCTTAPE_API TriggerAreaComponent : public Component {
    Q_OBJECT
public:
    /**
    * Constructor.
    * @param areaShape shape of the trigger area 
    * @param name The name of the component.
    */
    TriggerAreaComponent(btCollisionShape* areaShape,
                         const QString& name = "");

    void OnCreate();
    void OnDestroy();
    void OnEnable();
    void OnDisable();
    void OnUpdate(double time_diff);
    
    /**
    * Setter for the area shape
    * @param area Shape of the trigger area
    */
    void SetAreaShape(btCollisionShape* areaShape);

signals:
    void Triggered(dt::TriggerAreaComponent* trigger_area, dt::Node* node);

private:
    std::unique_ptr<btCollisionShape> mArea; /// area entering which sends the Triggered signal
    std::unique_ptr<btGhostObject> mObject;  /// object used to check collission

};

}

#endif