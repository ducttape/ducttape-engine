
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include <Physics/PhysicsWorld.hpp>

#include <Scene/Scene.hpp>
#include <Utils/Logger.hpp>

#include <OgreSceneManager.h>

namespace dt {

PhysicsWorld::PhysicsWorld(const QString& name, Scene* scene)
    : mDynamicsWorld(nullptr),
      mDebugDrawer(nullptr),
      mShowDebug(false),
      mScene(scene),
      mGravity(Ogre::Vector3(0, -9.8, 0)),
      mName(name),
      mIsEnabled(true) {}

void PhysicsWorld::Initialize() {
    Logger::Get().Info("Initializing phyics world: " + mName);

    // Manually create and manage memory for the Bullet stuff -- the Bullet way.
    mBroadphase = new btDbvtBroadphase();
    mCollisionConfiguration = new btDefaultCollisionConfiguration;
    mCollisionDispatcher = new btCollisionDispatcher(mCollisionConfiguration);
    mSolver = new btSequentialImpulseConstraintSolver();
    mDynamicsWorld = new btDiscreteDynamicsWorld(mCollisionDispatcher,
                                                 mBroadphase, mSolver,
                                                 mCollisionConfiguration);

    // setup world
    SetGravity(mGravity);
    mDynamicsWorld->setInternalTickCallback(PhysicsWorld::BulletTickCallback, static_cast<void *>(this));

    // setup debug drawer
    mDebugDrawer = new BtOgre::DebugDrawer(mScene->GetSceneManager()->getRootSceneNode(), mDynamicsWorld);
    mDebugDrawer->setDebugMode(mShowDebug);
    mDynamicsWorld->setDebugDrawer(mDebugDrawer);
}

void PhysicsWorld::Deinitialize() {
    // Delete in reverse order.
    delete mDebugDrawer;
    delete mDynamicsWorld;
    delete mSolver;
    delete mCollisionDispatcher;
    delete mCollisionConfiguration;
    delete mBroadphase;
}

void PhysicsWorld::StepSimulation(double time_diff) {
    if(mIsEnabled) {
        mDynamicsWorld->stepSimulation(time_diff, 10);
        mDebugDrawer->step();
    }
}

btDiscreteDynamicsWorld* PhysicsWorld::GetBulletWorld() {
    return mDynamicsWorld;
}

void PhysicsWorld::OnTick(btScalar time_diff) {
}

void PhysicsWorld::SetGravity(Ogre::Vector3 gravity) {
    mGravity = gravity;
    if(mDynamicsWorld != nullptr) {
        mDynamicsWorld->setGravity(BtOgre::Convert::toBullet(mGravity));
    }
}

const QString& PhysicsWorld::GetName() const {
    return mName;
}

void PhysicsWorld::SetShowDebug(bool show_debug) {
    mShowDebug = show_debug;
    if(mDebugDrawer != nullptr) {
        mDebugDrawer->setDebugMode(mShowDebug);
    }
}

bool PhysicsWorld::GetShowDebug() const {
    return mShowDebug;
}


void PhysicsWorld::SetEnabled(bool enabled) {
    mIsEnabled = enabled;
}

bool PhysicsWorld::IsEnabled() const {
    return mIsEnabled;
}

// Callback stuff for Bullet (static)
void PhysicsWorld::BulletTickCallback(btDynamicsWorld* world, btScalar time_diff) {
    PhysicsWorld* physics_world = static_cast<PhysicsWorld*>(world->getWorldUserInfo());
    physics_world->OnTick(time_diff);
}

}
