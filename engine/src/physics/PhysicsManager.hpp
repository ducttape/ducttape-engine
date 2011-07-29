
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_PHYSICS_PHYSICSMANAGER
#define DUCTTAPE_ENGINE_PHYSICS_PHYSICSMANAGER

#include <btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionShapes/btBox2dShape.h>
//#include <BulletCollision/CollisionDispatch/btEmptyCollisionAlgorithm.h>
//#include <BulletCollision/CollisionDispatch/btBox2dBox2dCollisionAlgorithm.h>
//#include <BulletCollision/CollisionDispatch/btConvex2dConvex2dAlgorithm.h>
//#include <BulletCollision/CollisionShapes/btConvex2dShape.h>
//#include <BulletCollision/NarrowPhaseCollision/btMinkowskiPenetrationDepthSolver.h>

#include "Manager.hpp"

namespace dt {

/**
  * A manager for keeping the physics world and for taking care of the complicated initialization.
  */
class PhysicsManager : public Manager {
public:
    /**
      * Default constructor.
      */
    PhysicsManager();

    void Initialize();
    void Deinitialize();
    static PhysicsManager* Get();
    btDiscreteDynamicsWorld* GetPhysicsWorld();

    void TickCallback(btScalar timestep);

private:
    btDbvtBroadphase* mBroadphase;
    btDefaultCollisionConfiguration* mCollisionConfiguration;
    btCollisionDispatcher* mCollisionDispatcher;
    //btVoronoiSimplexSolver* mSimplex;
    //btMinkowskiPenetrationDepthSolver* mPdSolver;
    btSequentialImpulseConstraintSolver* mSolver;
    btDiscreteDynamicsWorld* mDynamicsWorld;
    //btConvex2dConvex2dAlgorithm::CreateFunc> mConvexAlgo2d;
    //btBox2dBox2dCollisionAlgorithm::CreateFunc> mBox2dAlgo2d;

};

}

#endif
