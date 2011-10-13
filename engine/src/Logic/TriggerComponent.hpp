
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_COMPONENT_TRIGGERCOMPONENT
#define DUCTTAPE_ENGINE_COMPONENT_TRIGGERCOMPONENT

#include <Config.hpp>

#include <Event/Event.hpp>
#include <Scene/Component.hpp>

#include <QString>

namespace dt {

/**
  * A simple trigger component that triggers a callback on certain conditions.
  * @warning Class not complete.
  */
class DUCTTAPE_API TriggerComponent : public Component {
    Q_OBJECT
public:
    /**
      * Advanced constructor.
      * @param name The name for this component.
      * @param mesh_handle_d The handle for this component's debug mesh. The default one is trigger.mesh.
      * @see Component
      */
    TriggerComponent(const QString& name = "", const QString& mesh_handle_d = "trigger.mesh");

    virtual void HandleEvent(std::shared_ptr<Event> e);

    void OnCreate();
    void OnDestroy();
    void OnUpdate(double time_diff);
};

}

#endif
