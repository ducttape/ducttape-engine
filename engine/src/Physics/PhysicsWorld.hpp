
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_PHYSICS_PHYSICSWORLD
#define DUCTTAPE_ENGINE_PHYSICS_PHYSICSWORLD

#include <Config.hpp>

#include <Core/Manager.hpp>

#include <Physics/PhysicsBodyComponent.hpp>

#include <btBulletCollisionCommon.h>

#include <BtOgreExtras.h>

#include <QString>

namespace dt {

// forward declaration due to circular dependency
class Scene;

/**
  * Holds and manages a complete world of bullet objects and all associated instances.
  */
class DUCTTAPE_API PhysicsWorld : public Manager {
    Q_OBJECT
public:
    /**
      * Default constructor.
      * @param name The name for this PhysicsWorld.
      * @param scene The scene to link this world to.
      */
    PhysicsWorld(const QString& name, Scene* scene);


    void Initialize();

    void Deinitialize();

    /**
      * Steps the simulation.
      * @param time_diff The time to step the simulation with.
      */
    void StepSimulation(double time_diff);

    /**
      * Returns the bullet world.
      * @returns The bullet world.
      */
    btDiscreteDynamicsWorld* GetBulletWorld();

    /**
      * Callback called by the Bullet world itself. Do not call this manually.
      * @see PhysicsWorld::StepSimulation(double time_diff);
      * @param time_diff The time with which the world simulation was stepped.
      */
    void OnTick(btScalar time_diff);

    /**
      * Sets the Gravity in the world.
      * @param gravity The gravity vector. Default: (0, -9.81, 0)
      */
    void SetGravity(Ogre::Vector3 gravity);

    /**
      * Returns the name of this world.
      * @returns The name of this world.
      */
    const QString& GetName() const;

    /**
      * Sets whether debug drawings should be displayed.
      * @param show_debug Whether debug drawings should be displayed.
      */
    void SetShowDebug(bool show_debug);

    /**
      * Gets whether debug drawings are being displayed.
      * @returns Whether debug drawings are being displayed.
      */
    bool GetShowDebug() const;

    /**
      * Sets whether the world is enabled.
      * @param enabled Whether the world is enabled.
      */
    void SetEnabled(bool enabled);

    /**
      * Gets whether the world is enabled.
      * @returns Whether the world is enabled.
      */
    bool IsEnabled() const;

    /**
      * Bullet tick callback.
      * @param world The bullet world that was stepped.
      * @param time_diff The time the world was stepped with.
      */
    static void BulletTickCallback(btDynamicsWorld* world, btScalar time_diff);

private:
    // bullet stuff
    btDbvtBroadphase* mBroadphase;                              //!< The Bullet broadphase.
    btDefaultCollisionConfiguration* mCollisionConfiguration;   //!< The Bullet collision configuration.
    btCollisionDispatcher* mCollisionDispatcher;                //!< The Bullet collision dispatcher.
    btSequentialImpulseConstraintSolver* mSolver;               //!< The Bullet solver.
    btDiscreteDynamicsWorld* mDynamicsWorld;                    //!< The Bullet world.

    BtOgre::DebugDrawer* mDebugDrawer;  //!< The debug drawer.
    bool mShowDebug;                    //!< Whether to show debug drawings or not.
    Scene* mScene;                      //!< The scene associated with this PhysicsWorld.
    Ogre::Vector3 mGravity;             //!< The gravity of this world.
    QString mName;                  //!< The name of this world.
    bool mIsEnabled;                    //!< Whether the world is enabled or not.
};

}

#endif
