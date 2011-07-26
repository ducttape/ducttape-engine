
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include "Root.hpp"

#include "PhysicsManager.hpp"

namespace dt {

// Callback stuff for Bullet.
void worldTickCallback(btDynamicsWorld* world, btScalar timestep) {
    PhysicsManager* pm = static_cast<PhysicsManager *>(world->getWorldUserInfo());
    pm->TickCallback(timestep);
}

PhysicsManager::PhysicsManager() {
}

void PhysicsManager::Initialize() {
    // Manually create and manager memory for the Bullet stuff - the Bullet way.
    mBroadphase = new btDbvtBroadphase();
    mCollisionConfiguration = new btDefaultCollisionConfiguration;
    mCollisionDispatcher = new btCollisionDispatcher(mCollisionConfiguration);
    //mSimplex = new btVoronoiSimplexSolver();
    //mPdSolver = new btMinkowskiPenetrationDepthSolver();
    mSolver = new btSequentialImpulseConstraintSolver();
    mDynamicsWorld = new btDiscreteDynamicsWorld(mCollisionDispatcher,
                                                 mBroadphase, mSolver,
                                                 mCollisionConfiguration);

    mDynamicsWorld->setGravity(btVector3(0, -9.8, 0));

    //mConvexAlgo2d = boost::shared_ptr<btConvex2dConvex2dAlgorithm::CreateFunc>(new btConvex2dConvex2dAlgorithm::CreateFunc(mSimplex.get(), mPdSolver.get()));

    //mCollisionDispatcher->registerCollisionCreateFunc(CONVEX_2D_SHAPE_PROXYTYPE,CONVEX_2D_SHAPE_PROXYTYPE, mConvexAlgo2d.get());
    //mCollisionDispatcher->registerCollisionCreateFunc(BOX_2D_SHAPE_PROXYTYPE,CONVEX_2D_SHAPE_PROXYTYPE, mConvexAlgo2d.get());
    //mCollisionDispatcher->registerCollisionCreateFunc(CONVEX_2D_SHAPE_PROXYTYPE,BOX_2D_SHAPE_PROXYTYPE, mConvexAlgo2d.get());
    //mCollisionDispatcher->registerCollisionCreateFunc(BOX_2D_SHAPE_PROXYTYPE,BOX_2D_SHAPE_PROXYTYPE, new btBox2dBox2dCollisionAlgorithm::CreateFunc());
    //mCollisionDispatcher->registerCollisionCreateFunc(BOX_2D_SHAPE_PROXYTYPE,BOX_2D_SHAPE_PROXYTYPE, mBox2dAlgo2d.get());
    
    mDynamicsWorld->setInternalTickCallback(worldTickCallback, static_cast<void *>(this));

    // Set realistic gravity.
    mDynamicsWorld->setGravity(btVector3(0, -10, 0));

    //mDebugDraw = boost::shared_ptr<DebugDraw>(new DebugDraw(Root::get_mutable_instance().GetRenderWindow()));
    ////mDebugDraw->setDebugMode(btIDebugDraw::DBG_DrawWireframe);
    //mDebugDraw->setDebugMode(btIDebugDraw::DBG_MAX_DEBUG_DRAW_MODE);
    //mDynamicsWorld->setDebugDrawer(mDebugDraw);
}

void PhysicsManager::Deinitialize() {
    // Delete in reverse order.
    delete mDynamicsWorld;
    delete mSolver;
    delete mCollisionDispatcher;
    delete mCollisionConfiguration;
    delete mBroadphase;
}

PhysicsManager* PhysicsManager::Get() {
    return Root::get_mutable_instance().GetPhysicsManager();
}

btDiscreteDynamicsWorld* PhysicsManager::GetPhysicsWorld() {
    return mDynamicsWorld;
}

void PhysicsManager::TickCallback(btScalar timestep) {
}
//	int numManifolds = mDynamicsWorld->getDispatcher()->getNumManifolds();
//	for(int i = 0; i < numManifolds; ++i) {
//		btPersistentManifold* contactManifold =  mDynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
//		btCollisionObject* obA = static_cast<btCollisionObject*>(contactManifold->getBody0());
//		btCollisionObject* obB = static_cast<btCollisionObject*>(contactManifold->getBody1());
//		int numContacts = contactManifold->getNumContacts();
//		for (int j = 0; j < numContacts; ++j) {
//			btManifoldPoint& pt = contactManifold->getContactPoint(j);
//			if (pt.getDistance()<0.f) {
//				const btVector3& ptA = pt.getPositionWorldOnA();
//				const btVector3& ptB = pt.getPositionWorldOnB();
//				const btVector3& normalOnB = pt.m_normalWorldOnB;
//
//				if (obA->getUserPointer()!=NULL && obB->getUserPointer()!=NULL) {
//					Node* a = (Node*)obA->getUserPointer();
//					Node* b = (Node*)obB->getUserPointer();
//					/*Root& app = Root::get_mutable_instance();
//					if ( a->GetUID().substr(0,5)=="heart" && b->GetUID()=="player") {
//						app.SetWorldHearts(app.GetWorldHearts()+1);
//						DeleteEntityByUID(a->GetUID());
//					} else if (b->GetUID().substr(0,5)=="heart" && a->GetUID()=="player") {
//						app.SetWorldHearts(app.GetWorldHearts()+1);
//						DeleteEntityByUID(b->GetUID());
//					} */
//				}
//			}
//		}
//	}
//}

}
