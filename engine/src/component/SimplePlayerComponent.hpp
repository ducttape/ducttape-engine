
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_COMPONENT_SIMPLEPLAYERCOMPONENT
#define DUCTTAPE_ENGINE_COMPONENT_SIMPLEPLAYERCOMPONENT

#include <OGRE/OgreEntity.h>

#include "component/Component.hpp"

namespace dt {

/**
  * Moves the node when the WASD / Arrow keys are pressed and rotates it with the
  * mouse.
  */
class SimplePlayerComponent : public Component {
public:
    /**
      * Advanced constructor.
      * @param name The name of the Component.
      * @see Component
      */
    SimplePlayerComponent(const std::string& name = "");

    virtual void HandleEvent(std::shared_ptr<Event> e);

    void OnCreate();
    void OnDestroy();
    void OnUpdate(double time_diff);

    void SetWASDEnabled(bool wasd_enabled);
    bool GetWASDEnabled() const;

    void SetArrowsEnabled(bool arrows_enabled);
    bool GetArrowsEnabled() const;

    void SetMoveSpeed(float move_speed);
    float GetMoveSpeed() const;

    void SetMouseEnabled(bool mouse_enabled);
    bool GetMouseEnabled() const;

    void SetMouseSensitity(float mouse_sensitity);
    float GetMouseSensitity() const;

    void SetMouseYInversed(bool mouse_y_inversed);
    bool GetMouseYInversed() const;

private:
    bool mWASDEnabled;          //!< Whether the WASD  keys are enabled for movement or not.
    bool mArrowsEnabled;        //!< Whether the Arrow keys are enabled for movement or not.
    float mMoveSpeed;           //!< The speed to move at. Default: 5.0. In units per second.
    bool mMouseEnabled;         //!< Whether the Mouse is enabled for looking around or not.
    float mMouseSensitivity;    //!< The sensitivity of the mouse. Default: 1.0.
    bool mMouseYInversed;       //!< True if the mouse's y-axis should be inversed.

};

}

#endif
