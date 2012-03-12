
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include <Physics/PhysicsWorld.hpp>

#include <Scene/Scene.hpp>
#include <Utils/Logger.hpp>

#include <BulletCollision/CollisionDispatch/btGhostObject.h>

#include <OgreSceneManager.h>

namespace dt {

PhysicsWorld::PhysicsWorld(const QString name, Scene* scene)
    : mDynamicsWorld(nullptr),
      mDebugDrawer(nullptr),
      mShowDebug(false),
      mScene(scene),
      mGravity(Ogre::Vector3(0, -9.8, 0)),
      mName(name),
      mIsEnabled(true) {}

void PhysicsWorld::initialize() {
    Logger::get().info("Initializing phyics world: " + mName);

    // Manually create and manage memory for the Bullet stuff -- the Bullet way.
    mBroadphase = new btDbvtBroadphase();
    mCollisionConfiguration = new btDefaultCollisionConfiguration;
    mCollisionDispatcher = new btCollisionDispatcher(mCollisionConfiguration);
    mSolver = new btSequentialImpulseConstraintSolver();
    mDynamicsWorld = new btDiscreteDynamicsWorld(mCollisionDispatcher,
                                                 mBroadphase, mSolver,
                                                 mCollisionConfiguration);

    // setup world
    setGravity(mGravity);
    mDynamicsWorld->setInternalTickCallback(PhysicsWorld::BulletTickCallback, static_cast<void *>(this));

    // setup debug drawer
    mDebugDrawer = new BtOgre::DebugDrawer(mScene->getSceneManager()->getRootSceneNode(), mDynamicsWorld);
    mDebugDrawer->setDebugMode(mShowDebug);
    mDynamicsWorld->setDebugDrawer(mDebugDrawer);

    mDynamicsWorld->getBroadphase()->getOverlappingPairCache()->setInternalGhostPairCallback(new btGhostPairCallback());
}

void PhysicsWorld::deinitialize() {
    // Delete in reverse order.
    delete mDebugDrawer;
    delete mDynamicsWorld;
    delete mSolver;
    delete mCollisionDispatcher;
    delete mCollisionConfiguration;
    delete mBroadphase;
}

void PhysicsWorld::stepSimulation(double time_diff) {
    if(mIsEnabled) {
        mDynamicsWorld->stepSimulation(time_diff, 10);
        mDebugDrawer->step();
    }
}

btDiscreteDynamicsWorld* PhysicsWorld::getBulletWorld() {
    return mDynamicsWorld;
}

void PhysicsWorld::onTick(btScalar time_diff) {
    uint32_t num_manifolds = mDynamicsWorld->getDispatcher()->getNumManifolds();
    for(uint32_t i = 0; i < num_manifolds; ++i) {
        btPersistentManifold* contact_manifold =  mDynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
        btCollisionObject* ob_a = static_cast<btCollisionObject*>(contact_manifold->getBody0());
        btCollisionObject* ob_b = static_cast<btCollisionObject*>(contact_manifold->getBody1());

        uint32_t num_contacts = contact_manifold->getNumContacts();
        for(uint32_t j = 0; j < num_contacts; ++j) {
            btManifoldPoint& pt = contact_manifold->getContactPoint(j);

            if(pt.getDistance() < 0.f) {
                //const btVector3& pt_a = pt.getPositionWorldOnA();
                //const btVector3& pt_b = pt.getPositionWorldOnB();
                //const btVector3& normal_on_b = pt.m_normalWorldOnB;

                if(ob_a->getUserPointer() != nullptr && ob_b->getUserPointer() != nullptr) {
                    PhysicsBodyComponent* physics_body_a = (PhysicsBodyComponent*)ob_a->getUserPointer();
                    PhysicsBodyComponent* physics_body_b = (PhysicsBodyComponent*)ob_b->getUserPointer();

                    physics_body_a->onCollide(physics_body_b);
                    physics_body_b->onCollide(physics_body_a);
                }
            }
        }
    }
}

void PhysicsWorld::setGravity(Ogre::Vector3 gravity) {
    mGravity = gravity;
    if(mDynamicsWorld != nullptr) {
        mDynamicsWorld->setGravity(BtOgre::Convert::toBullet(mGravity));
    }
}

const QString PhysicsWorld::getName() const {
    return mName;
}

void PhysicsWorld::setShowDebug(bool show_debug) {
    mShowDebug = show_debug;
    if(mDebugDrawer != nullptr) {
        mDebugDrawer->setDebugMode(mShowDebug);
    }
}

bool PhysicsWorld::getShowDebug() const {
    return mShowDebug;
}


void PhysicsWorld::setEnabled(bool enabled) {
    mIsEnabled = enabled;
}

bool PhysicsWorld::isEnabled() const {
    return mIsEnabled;
}

// Callback stuff for Bullet (static)
void PhysicsWorld::BulletTickCallback(btDynamicsWorld* world, btScalar time_diff) {
    PhysicsWorld* physics_world = static_cast<PhysicsWorld*>(world->getWorldUserInfo());
    physics_world->onTick(time_diff);
}

}
