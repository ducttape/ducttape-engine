
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include <Physics/PhysicsBodyComponent.hpp>

#include <Graphics/MeshComponent.hpp>
#include <Scene/Node.hpp>
#include <Scene/Scene.hpp>

#include <BtOgreGP.h>

#include <BulletCollision/CollisionShapes/btBoxShape.h>

namespace dt {
PhysicsBodyComponent::PhysicsBodyComponent(const QString mesh_component_name,
                                           const QString name,
                                           CollisionShapeType collision_shape_type,
                                           btScalar mass)
    : Component(name),
      mMeshComponentName(mesh_component_name),
      mCollisionShape(nullptr),
      mCollisionShapeType(collision_shape_type),
      mBody(nullptr),
      mMotionState(nullptr),
      mCentralForce(btVector3(0, 0, 0)),
      mTorque(btVector3(0, 0, 0)),
      mCollisionMask(0),
      mCollisionGroup(0),
      mCollisionMaskInUse(false),
      mMass(mass) {}

void PhysicsBodyComponent::onInitialize() {
    if(! mNode->hasComponent(mMeshComponentName)) {
        Logger::get().error("Node " + mNode->getName() + " has no Component named " +
                            mMeshComponentName + " which is required to create the" +
                            " PhysicsBodyComponent " + mName);
        exit(1);
    }

    auto mesh_component = mNode->findComponent<MeshComponent>(mMeshComponentName);

    BtOgre::StaticMeshToShapeConverter converter(mesh_component->getOgreEntity());
    
    // TODO: CollisionShapes should likely be stored at a central place.
    // Perhaps the PhysicsManager is a good place. It would save a lot of memory
    // for many bodies with the same CollisionShape.
    if(mCollisionShapeType == BOX) {
        Ogre::Vector3 size = mesh_component->getOgreEntity()->getBoundingBox().getSize();
        size /= 2.0;
        mCollisionShape = new btBoxShape(BtOgre::Convert::toBullet(size));
        //mCollisionShape = converter.createBox();
    }
    else if(mCollisionShapeType == CONVEX)
        mCollisionShape = converter.createConvex();
    else if(mCollisionShapeType == SPHERE) {
        mCollisionShape = new btSphereShape(mesh_component->getOgreEntity()->getBoundingRadius());
    }
    else if(mCollisionShapeType == CYLINDER) {
        Ogre::Vector3 size = mesh_component->getOgreEntity()->getBoundingBox().getSize();
        size /= 2.0;
        mCollisionShape = new btCylinderShape(BtOgre::Convert::toBullet(size));
    }
    else if(mCollisionShapeType == TRIMESH)
        mCollisionShape = converter.createTrimesh();

    btVector3 inertia(0, 0, 0);
    //Only the rigidbody's mass doesn't equal to zero is dynamic or some odd phenomenon may appear.
    if(mMass != 0.0f)
        mCollisionShape->calculateLocalInertia(mMass, inertia);

    btDefaultMotionState* state = new btDefaultMotionState(
        btTransform(BtOgre::Convert::toBullet(getNode()->getRotation(Node::SCENE)),
        BtOgre::Convert::toBullet(getNode()->getPosition(Node::SCENE))));

    //Here's the most tricky part. You need to give it 5.0 as its temporary mass.
    //Or you will find your player character keeps falling down no matter there's a ground.
    //Its real mass will be set in OnEnable().
    mBody = new btRigidBody(5.0, state, mCollisionShape, inertia);

    // Store pointer to this PhysicsBodyComponent for later retrieval (for
    // collisions, for instance)
    mBody->setFriction(1.0);
    mBody->setUserPointer((void *)(this));
}

void PhysicsBodyComponent::SetFriction(double friction) {
    mBody->setFriction(friction);
}

double PhysicsBodyComponent::GetFriction() const {
    return mBody->getFriction();
}

void PhysicsBodyComponent::onDeinitialize() {
    delete mBody->getMotionState();
    delete mBody;
    delete mCollisionShape;
}

void PhysicsBodyComponent::onEnable() {
    if(mCollisionMaskInUse) // Special treatment due to Bullet internals
        getNode()->getScene()->getPhysicsWorld()->getBulletWorld()->addRigidBody(mBody, mCollisionGroup, mCollisionMask);
    else
        getNode()->getScene()->getPhysicsWorld()->getBulletWorld()->addRigidBody(mBody);

    //Re-sychronize the PhysicsBodyComponent with the node.
    btDefaultMotionState* state = new btDefaultMotionState(
        btTransform(BtOgre::Convert::toBullet(getNode()->getRotation(Node::SCENE)),
        BtOgre::Convert::toBullet(getNode()->getPosition(Node::SCENE))));
    mBody->setMotionState(state);

    setMass(mMass);

    //Activate it.
    this->activate();
}

void PhysicsBodyComponent::onDisable() {
    getNode()->getScene()->getPhysicsWorld()->getBulletWorld()->removeRigidBody(mBody);
}

void PhysicsBodyComponent::onCollide(PhysicsBodyComponent* other_body) {
    emit collided(other_body, this);
}

btRigidBody* PhysicsBodyComponent::getRigidBody() {
    return mBody;
}

void PhysicsBodyComponent::applyCentralImpulse(const btVector3& impulse) {
    mBody->applyCentralImpulse(impulse);
}

void PhysicsBodyComponent::applyCentralImpulse(float x, float y, float z) {
    applyCentralImpulse(btVector3(x, y, z));
}

void PhysicsBodyComponent::setCentralForce(const btVector3& force) {
    mCentralForce = force;
}

void PhysicsBodyComponent::setCentralForce(float x, float y, float z) {
    setCentralForce(btVector3(x, y, z));
}

void PhysicsBodyComponent::setTorque(const btVector3& torque) {
    mTorque = torque;
}

void PhysicsBodyComponent::setTorque(float x, float y, float z) {
    setTorque(btVector3(x, y, z));
}

void PhysicsBodyComponent::setCollisionMask(uint16_t collision_mask) {
    mCollisionMask = collision_mask;
    mCollisionMaskInUse = true;

    if(isEnabled()) {
        disable();
        enable();
    }
}

void PhysicsBodyComponent::setCollisionGroup(uint16_t collision_group) {
    mCollisionGroup = collision_group;
    mCollisionMaskInUse = true;

    if(isEnabled()) {
        disable();
        enable();
    }
}

const btVector3 PhysicsBodyComponent::getCentralForce() const {
    return mCentralForce;
}

const btVector3 PhysicsBodyComponent::getTorque() const {
    return mTorque;
}

void PhysicsBodyComponent::setRestrictMovement(const btVector3& restriction) {
    mBody->setLinearFactor(restriction);
}

void PhysicsBodyComponent::setRestrictMovement(float x, float y, float z) {
    setRestrictMovement(btVector3(x, y, z));
}

void PhysicsBodyComponent::setRestrictRotation(const btVector3& restriction) {
    mBody->setAngularFactor(restriction);
}

void PhysicsBodyComponent::setRestrictRotation(float x, float y, float z) {
    setRestrictRotation(btVector3(x, y, z));
}

void PhysicsBodyComponent::setGravity(const btVector3& gravity) {
    mBody->setGravity(gravity);
}

void PhysicsBodyComponent::setGravity(float x, float y, float z) {
    setGravity(btVector3(x, y, z));
}

void PhysicsBodyComponent::disableSleep(bool disabled) {
    if(disabled) {
        mBody->setActivationState(DISABLE_DEACTIVATION);
    }
}

void PhysicsBodyComponent::setDampingAmount(btScalar lindamping, btScalar angdamping) {
    mBody->setDamping(lindamping, angdamping);
}

void PhysicsBodyComponent::onUpdate(double time_diff) {
    btTransform trans;
    mBody->getMotionState()->getWorldTransform(trans);
    /*std::cout << "x: " << trans.getOrigin().getX() << " y: "
        << trans.getOrigin().getY() << " z: " << trans.getOrigin().getZ() << std::endl;*/

    if(!mCentralForce.isZero()) {
        mBody->applyCentralForce(mCentralForce);
    }

    if(!mTorque.isZero()) {
        mBody->applyTorque(mTorque);
    }

    getNode()->setPosition(BtOgre::Convert::toOgre(trans.getOrigin()), Node::SCENE);
    getNode()->setRotation(BtOgre::Convert::toOgre(trans.getRotation()), Node::SCENE);
}

void PhysicsBodyComponent::setMass(btScalar mass) {
    btVector3 inertia(0, 0, 0);
    if(mass != 0.0f)
        mCollisionShape->calculateLocalInertia(mass, inertia);
    mBody->setMassProps(mass, inertia);
    mMass = mass;
}
/*
void PhysicsBodyComponent::SetCollisionShapeType(CollisionShapeType type) {
    mCollisionShapeType = type;
}
*/

void PhysicsBodyComponent::activate() {
    mBody->activate();
}
}
