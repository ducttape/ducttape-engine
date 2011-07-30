
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_PHYSICS_PHYSICSWORLD
#define DUCTTAPE_ENGINE_PHYSICS_PHYSICSWORLD

#include <OGRE/OgreVector3.h>
#include <OGRE/OgreSceneNode.h>

#include <BtOgreExtras.h>
#include <btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionShapes/btBox2dShape.h>

#include "Config.hpp"

namespace dt {

// forward declaration
class Scene;

/**
  * Holds and manages a complete world of bullet objects and all associated instances.
  */
class DUCTTAPE_API PhysicsWorld {
public:
    /**
      * Default constructor.
      */
    PhysicsWorld(const std::string& name, Scene* scene);

    void Initialize();

    void Deinitialize();

    void StepSimulation(double time_diff);

    btDiscreteDynamicsWorld* GetBulletWorld();

    /**
      * Callback called by the Bullet world itself. Do not call this manually.
      * @see PhysicsWorld::StepSimulation(double time_diff);
      * @param time_diff The time with which the world simulation was stepped.
      */
    void OnTick(btScalar time_diff);

    void SetGravity(Ogre::Vector3 gravity);

    const std::string& GetName() const;

    void SetShowDebug(bool show_debug);

    bool GetShowDebug() const;

    void SetEnabled(bool enabled);

    bool IsEnabled() const;

    static void BulletTickCallback(btDynamicsWorld* world, btScalar time_diff);

private:
    // bullet stuff
    btDbvtBroadphase* mBroadphase;
    btDefaultCollisionConfiguration* mCollisionConfiguration;
    btCollisionDispatcher* mCollisionDispatcher;
    btSequentialImpulseConstraintSolver* mSolver;
    btDiscreteDynamicsWorld* mDynamicsWorld;

    BtOgre::DebugDrawer* mDebugDrawer;
    bool mShowDebug;
    Scene* mScene;
    Ogre::Vector3 mGravity;
    std::string mName;
    bool mIsEnabled;
};

}

#endif
