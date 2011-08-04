
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_COMPONENT_SIMPLEPLAYERCOMPONENT
#define DUCTTAPE_ENGINE_COMPONENT_SIMPLEPLAYERCOMPONENT

#include <Config.hpp>

#include <Event/Event.hpp>
#include <Scene/Component.hpp>

#include <QString>

#include <memory>

namespace dt {

/**
  * Moves the node when the WASD / Arrow keys are pressed and rotates it with the
  * mouse.
  */
class DUCTTAPE_API SimplePlayerComponent : public Component {
public:
    /**
      * Advanced constructor.
      * @param name The name of the Component.
      * @see Component
      */
    SimplePlayerComponent(const QString& name = "");

    virtual void HandleEvent(std::shared_ptr<Event> e);

    void OnCreate();
    void OnDestroy();
    void OnUpdate(double time_diff);

    /**
      * Sets whether the WASD keys are enabled for movement.
      * @param wasd_enabled Whether the WASD keys are enabled for movement.
      */
    void SetWASDEnabled(bool wasd_enabled);

    /**
      * Gets whether the WASD keys are enabled for movement.
      * @returns Whether the WASD keys are enabled for movement.
      */
    bool GetWASDEnabled() const;

    /**
      * Sets whether the arrow keys are enabled for movement.
      * @param arrows_enabled Whether the arrow keys are enabled for movement.
      */
    void SetArrowsEnabled(bool arrows_enabled);

    /**
      * Gets whether the arrow keys are enabled for movement.
      * @returns Whether the arrow keys are enabled for movement.
      */
    bool GetArrowsEnabled() const;

    /**
      * Sets the speed the player moves at.
      * @param move_speed The maximum speed, in units per second.
      */
    void SetMoveSpeed(float move_speed);

    /**
      * Gets the speed the player moves at.
      * @returns The player speed, in units per second.
      */
    float GetMoveSpeed() const;

    /**
      * Sets whether the mouse is enabled for looking around.
      * @param mouse_enabled Whether the mouse is enabled for looking around.
      */
    void SetMouseEnabled(bool mouse_enabled);

    /**
      * Gets whether the mouse is enabled for looking around.
      * @returns Whether the mouse is enabled for looking around.
      */
    bool GetMouseEnabled() const;

    /**
      * Sets the mouse sensitivity. Default: 1.0.
      * @param mouse_sensitivity The mouse sensitivity. Default: 1.0.
      */
    void SetMouseSensitivity(float mouse_sensitivity);

    /**
      * Gets the mouse sensitivity. Default: 1.0.
      * @returns The mouse sensitivity. Default: 1.0.
      */
    float GetMouseSensitivity() const;

    /**
      * Sets whether the mouse y-axis should be inversed.
      * @param mouse_y_inversed Whether the mouse y-axis should be inversed.
      */
    void SetMouseYInversed(bool mouse_y_inversed);

    /**
      * Gets whether the mouse y-axis should be inversed.
      * @returns Whether the mouse y-axis should be inversed.
      */
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
