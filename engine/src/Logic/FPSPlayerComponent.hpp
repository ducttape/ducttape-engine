
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_COMPONENT_FPSPLAYERCOMPONENT
#define DUCTTAPE_ENGINE_COMPONENT_FPSPLAYERCOMPONENT

#include <Config.hpp>

#include <Scene/Component.hpp>

#include <BulletCollision/CollisionShapes/btConvexHullShape.h>
#include <btBulletDynamicsCommon.h>
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
class DUCTTAPE_API FPSPlayerComponent : public Component {
    Q_OBJECT
public:
    /**
      * Advanced constructor.
      * @param name The name of the Component.
      * @see Component
      */
    FPSPlayerComponent(const QString& name = "");

    void OnCreate();
    void OnDestroy();
    void OnEnable();
    void OnDisable();
    void OnUpdate(double time_diff);

private slots:
    void _HandleKeyPressed(const OIS::KeyEvent& event);

private:
    btKinematicCharacterController* mBtController;       //!< The bullet's built-in character controller.
    btPairCachingGhostObject* mBtGhostObject;            //!< The bullet's ghost object.
};
}

#endif