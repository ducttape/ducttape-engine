// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_COMPONENT_SIMPLEPLAYERCOMPONENT
#define DUCTTAPE_ENGINE_COMPONENT_SIMPLEPLAYERCOMPONENT

#include <Config.hpp>

#include <Scene/Component.hpp>
#include <Input/InputManager.hpp>

#define OIS_DYNAMIC_LIB
#include <OIS.h>

#include <OgreMatrix3.h>

#include <QString>

#include <memory>

namespace dt {

/**
  * Moves the node when the WASD / Arrow keys are pressed and rotates it with the
  * mouse.
  */
class DUCTTAPE_API SimplePlayerComponent : public Component {
    Q_OBJECT
public:
    DT_SERIALIZABLE(SimplePlayerComponent)
    /**
      * Advanced constructor.
      * @param name The name of the Component.
      * @see Component
      */
    SimplePlayerComponent(const QString name = "");

    //virtual void HandleEvent(std::shared_ptr<Event> e);

    void onInitialize();
    void onDeinitialize();
    void onUpdate(double time_diff);

    /**
      * Sets whether the WASD keys are enabled for movement.
      * @param wasd_enabled Whether the WASD keys are enabled for movement.
      */
    void setWASDEnabled(bool wasd_enabled);

    /**
      * Gets whether the WASD keys are enabled for movement.
      * @returns Whether the WASD keys are enabled for movement.
      */
    bool getWASDEnabled() const;

    /**
      * Sets whether the arrow keys are enabled for movement.
      * @param arrows_enabled Whether the arrow keys are enabled for movement.
      */
    void setArrowsEnabled(bool arrows_enabled);

    /**
      * Gets whether the arrow keys are enabled for movement.
      * @returns Whether the arrow keys are enabled for movement.
      */
    bool getArrowsEnabled() const;

    /**
      * Sets the speed the player moves at.
      * @param move_speed The maximum speed, in units per second.
      */
    void setMoveSpeed(float move_speed);

    /**
      * Gets the speed the player moves at.
      * @returns The player speed, in units per second.
      */
    float getMoveSpeed() const;

    /**
      * Sets whether the mouse is enabled for looking around.
      * @param mouse_enabled Whether the mouse is enabled for looking around.
      */
    void setMouseEnabled(bool mouse_enabled);

    /**
      * Gets whether the mouse is enabled for looking around.
      * @returns Whether the mouse is enabled for looking around.
      */
    bool getMouseEnabled() const;

    /**
      * Sets the mouse sensitivity. Default: 1.0.
      * @param mouse_sensitivity The mouse sensitivity. Default: 1.0.
      */
    void setMouseSensitivity(float mouse_sensitivity);

    /**
      * Gets the mouse sensitivity. Default: 1.0.
      * @returns The mouse sensitivity. Default: 1.0.
      */
    float getMouseSensitivity() const;

    /**
      * Sets whether the mouse y-axis should be inversed.
      * @param mouse_y_inversed Whether the mouse y-axis should be inversed.
      */
    void setMouseYInversed(bool mouse_y_inversed);

    /**
      * Gets whether the mouse y-axis should be inversed.
      * @returns Whether the mouse y-axis should be inversed.
      */
    bool getMouseYInversed() const;
    
    /**
      * Sets whether the node will move until the key is released.
      * @param pressed Whether the node will move until the key is released.
      */
    void setConstant(bool constant);
    
    /**
      * Gets whether the node will move until the key is released.
      * @returns consta Whether the node will move until the key is released.
      */
    bool getConstant() const;

private slots:
    void _handleMouseInput(const OIS::MouseEvent& event);

    void _handleKeyPressed(dt::InputManager::InputCode input_code, const OIS::EventArg& event);
    void _handleKeyReleased(dt::InputManager::InputCode input_code, const OIS::EventArg& event);

private:
    bool mWASDEnabled;          //!< Whether the WASD  keys are enabled for movement or not.
    bool mArrowsEnabled;        //!< Whether the Arrow keys are enabled for movement or not.
    float mMoveSpeed;           //!< The speed to move at. Default: 5.0. In units per second.
    bool mMouseEnabled;         //!< Whether the Mouse is enabled for looking around or not.
    float mMouseSensitivity;    //!< The sensitivity of the mouse. Default: 1.0.
    bool mMouseYInversed;       //!< True if the mouse's y-axis should be inversed.
    bool mCostant;              //!< If true, the node will move until the key is released.
    bool mInputStateChanged;    //!< True if one key is pressed, released or the mouse is moved.
    Ogre::Vector3 mMove;        //!< Record the movement of the node. It will be set to zero after every update of the node.

};

}

#endif
