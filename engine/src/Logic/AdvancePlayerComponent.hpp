
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_COMPONENT_ADVANCEPLAYERCOMPONENT
#define DUCTTAPE_ENGINE_COMPONENT_ADVANCEPLAYERCOMPONENT

#include <Config.hpp>

#include <Scene/Component.hpp>

#include <BulletCollision/CollisionShapes/btConvexHullShape.h>
#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>
#include <BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include <BulletDynamics/Character/btKinematicCharacterController.h>

#define OIS_DYNAMIC_LIB
#include <OIS.h>

#include <QString>

namespace dt {

/**
  * A component to deal with the input using bullet's built-in character controller.
  */
class DUCTTAPE_API AdvancePlayerComponent : public Component {
    Q_OBJECT
public:
    /**
      * Advanced constructor.
      * @param name The name of the Component.
      * @see Component
      */
    AdvancePlayerComponent(const QString& name = "");

    void OnCreate();
    void OnDestroy();
    void OnEnable();
    void OnDisable();
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

    /**
      * Sets whether the character can jump or not.
      * @param jump_enabled Whether the character can jump or not.
      */
    void SetJumpEnabled(bool jump_enabled);

    /**
      * Gets whether the character can jump or not.
      * @returns Whether the character can jump or not.
      */
    bool GetJumpEnabled() const;

    /**
      * Sets the InteractionComponent's name.
      * @param name The name of the InteractionComponent to set.
      */
    void SetInteractionComponentName(const QString& name);

    /**
      * Gets the InteractionComponent's name.
      * @returns The name of the InteractionComponent.
      */
    QString GetInteractionComponentName() const;

    /**
      * Gets whether the mouse's key pressing effect is one-shot or not.
      * @returns Whether the mouse's key pressing effect is one-shot or not.
      */
    bool GetIsOneShot() const;

    /**
      * Sets whether the mouse's key pressing effect is one-shot or not.
      * @param is_one_shot Whether the mouse's key pressing effect is one-shot or not.
      */
    void SetIsOneShot(bool is_one_shot);

private slots:
    void _HandleKeyDown(const OIS::KeyEvent& event);

    void _HandleMouseMove(const OIS::MouseEvent& event);

    void _HandleKeyUp(const OIS::KeyEvent& event);

    void _HandleMouseDown(const OIS::MouseEvent& event, OIS::MouseButtonID button);

    void _HandleMouseUp(const OIS::MouseEvent& event, OIS::MouseButtonID button);

private:
    btKinematicCharacterController* mBtController;       //!< The bullet's built-in character controller.
    btPairCachingGhostObject* mBtGhostObject;            //!< The bullet's ghost object.;
    bool mMouseEnabled;         //!< Whether the Mouse is enabled for looking around or not.
    float mMouseSensitivity;    //!< The sensitivity of the mouse. Default: 1.0.
    bool mMouseYInversed;       //!< True if the mouse's y-axis should be inversed.
    btVector3 mMove;            //!< Used to keep track of the movement.
    float mMoveSpeed;           //!< The speed to move at. Default: 5.0. In units per second.
    bool mWASDEnabled;          //!< Whether the WASD  keys are enabled for movement or not.
    bool mArrowsEnabled;        //!< Whether the Arrow keys are enabled for movement or not.
    bool mJumpEnabled;          //!< Whether the character can jump or not.
    QString mInteractionComponentName;  //!< The name of the InteractionComponent to deal with the interaction with other objects.
    bool mIsMouseLeftDown;     //!< Whether the mouse's left key is pressed or not.
    bool mIsOneShot;            //!< Whether the mouse's key pressing effect is one-shot or not.
};
}

#endif