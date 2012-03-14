
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
PhysicsBodyComponent::PhysicsBodyComponent(const QString& mesh_component_name, 
                                           const QString& name,
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

void PhysicsBodyComponent::OnInitialize() {
    if(! mNode->HasComponent(mMeshComponentName)) {
        Logger::Get().Error("Node "+mNode->GetName()+" has no Component named "+
                            mMeshComponentName+" which is required to create the"+
                            " PhysicsBodyComponent "+mName);
        exit(1);
    }

    MeshComponent* mesh_component =
        mNode->FindComponent<MeshComponent>(mMeshComponentName);

    BtOgre::StaticMeshToShapeConverter converter(mesh_component->GetOgreEntity());
    
    // TODO: CollisionShapes should likely be stored at a central place.
    // Perhaps the PhysicsManager is a good place. It would save a lot of memory
    // for many bodies with the same CollisionShape.
    if(mCollisionShapeType == BOX) {
        Ogre::Vector3 size = mesh_component->GetOgreEntity()->getBoundingBox().getSize();
        size /= 2.0;
        mCollisionShape = new btBoxShape(BtOgre::Convert::toBullet(size));
        //mCollisionShape = converter.createBox();
    }
    else if(mCollisionShapeType == CONVEX)
        mCollisionShape = converter.createConvex();
    else if(mCollisionShapeType == SPHERE) {
        mCollisionShape = new btSphereShape(mesh_component->GetOgreEntity()->getBoundingRadius());
    }
    else if(mCollisionShapeType == CYLINDER) {
        Ogre::Vector3 size = mesh_component->GetOgreEntity()->getBoundingBox().getSize();
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
        btTransform(BtOgre::Convert::toBullet(GetNode()->GetRotation(Node::SCENE)),
        BtOgre::Convert::toBullet(GetNode()->GetPosition(Node::SCENE))));

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

void PhysicsBodyComponent::OnDeinitialize() {
    delete mBody->getMotionState();
    delete mBody;
    delete mCollisionShape;
}

void PhysicsBodyComponent::OnEnable() {
    if(mCollisionMaskInUse) // Special treatment due to Bullet internals
        GetNode()->GetScene()->GetPhysicsWorld()->GetBulletWorld()->addRigidBody(mBody, mCollisionGroup, mCollisionMask);
    else
        GetNode()->GetScene()->GetPhysicsWorld()->GetBulletWorld()->addRigidBody(mBody);

    //Re-sychronize the PhysicsBodyComponent with the node.
    btDefaultMotionState* state = new btDefaultMotionState(
        btTransform(BtOgre::Convert::toBullet(GetNode()->GetRotation(Node::SCENE)),
        BtOgre::Convert::toBullet(GetNode()->GetPosition(Node::SCENE))));
    mBody->setMotionState(state);

    SetMass(mMass);

    //Activate it.
    this->Activate();
}

void PhysicsBodyComponent::OnDisable() {
    GetNode()->GetScene()->GetPhysicsWorld()->GetBulletWorld()->removeRigidBody(mBody);
}

void PhysicsBodyComponent::OnCollide(PhysicsBodyComponent* other_body) {
    emit Collided(other_body, this);
}

btRigidBody* PhysicsBodyComponent::GetRigidBody() {
    return mBody;
}

void PhysicsBodyComponent::ApplyCentralImpulse(const btVector3& impulse) {
    mBody->applyCentralImpulse(impulse);
}

void PhysicsBodyComponent::ApplyCentralImpulse(float x, float y, float z) {
    ApplyCentralImpulse(btVector3(x, y, z));
}

void PhysicsBodyComponent::SetCentralForce(const btVector3& force) {
    mCentralForce = force;
}

void PhysicsBodyComponent::SetCentralForce(float x, float y, float z) {
    SetCentralForce(btVector3(x, y, z));
}

void PhysicsBodyComponent::SetTorque(const btVector3& torque) {
    mTorque = torque;
}

void PhysicsBodyComponent::SetTorque(float x, float y, float z) {
    SetTorque(btVector3(x, y, z));
}

void PhysicsBodyComponent::SetCollisionMask(uint16_t collision_mask) {
    mCollisionMask = collision_mask;
    mCollisionMaskInUse = true;

    if(IsEnabled()) {
        Disable();
        Enable();
    }
}

void PhysicsBodyComponent::SetCollisionGroup(uint16_t collision_group) {
    mCollisionGroup = collision_group;
    mCollisionMaskInUse = true;

    if(IsEnabled()) {
        Disable();
        Enable();
    }
}

const btVector3 PhysicsBodyComponent::GetCentralForce() const {
    return mCentralForce;
}

const btVector3 PhysicsBodyComponent::GetTorque() const {
    return mTorque;
}

void PhysicsBodyComponent::SetRestrictMovement(const btVector3& restriction) {
    mBody->setLinearFactor(restriction);
}

void PhysicsBodyComponent::SetRestrictMovement(float x, float y, float z) {
    SetRestrictMovement(btVector3(x, y, z));
}

void PhysicsBodyComponent::SetRestrictRotation(const btVector3& restriction) {
    mBody->setAngularFactor(restriction);
}

void PhysicsBodyComponent::SetRestrictRotation(float x, float y, float z) {
    SetRestrictRotation(btVector3(x, y, z));
}

void PhysicsBodyComponent::SetGravity(const btVector3& gravity) {
    mBody->setGravity(gravity);
}

void PhysicsBodyComponent::SetGravity(float x, float y, float z) {
    SetGravity(btVector3(x, y, z));
}

void PhysicsBodyComponent::DisableSleep(bool disabled) {
    if(disabled) {
        mBody->setActivationState(DISABLE_DEACTIVATION);
    }
}

void PhysicsBodyComponent::SetDampingAmount(btScalar lindamping, btScalar angdamping) {
    mBody->setDamping(lindamping, angdamping);
}

void PhysicsBodyComponent::OnUpdate(double time_diff) {
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

    GetNode()->SetPosition(BtOgre::Convert::toOgre(trans.getOrigin()), Node::SCENE);
    GetNode()->SetRotation(BtOgre::Convert::toOgre(trans.getRotation()), Node::SCENE);
}

void PhysicsBodyComponent::SetMass(btScalar mass) {
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

void PhysicsBodyComponent::Activate() {
    mBody->activate();
}
}
