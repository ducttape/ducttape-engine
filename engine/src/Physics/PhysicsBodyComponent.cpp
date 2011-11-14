
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

namespace dt {

PhysicsBodyComponent::PhysicsBodyComponent(const QString& mesh_component_name,
                                           const QString& name)
    : Component(name),
      mMeshComponentName(mesh_component_name),
      mCollisionShape(nullptr),
      mCollisionShapeType(CONVEX),
      mBody(nullptr),
      mMotionState(nullptr),
      mCentralForce(btVector3(0, 0, 0)),
      mTorque(btVector3(0, 0, 0)),
      mCollisionMask(0),
      mCollisionGroup(0),
      mCollisionMaskInUse(false) {}

void PhysicsBodyComponent::OnCreate() {
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
    if(mCollisionShapeType == BOX)
        mCollisionShape = converter.createBox();
    else if(mCollisionShapeType == CONVEX)
        mCollisionShape = converter.createConvex();
    else if(mCollisionShapeType == SPHERE)
        mCollisionShape = converter.createSphere();
    else if(mCollisionShapeType == CYLINDER)
        mCollisionShape = converter.createCylinder();
    else if(mCollisionShapeType == TRIMESH)
        mCollisionShape = converter.createTrimesh();


    btScalar mass = 5;
    btVector3 inertia;
    mCollisionShape->calculateLocalInertia(mass, inertia);

    btDefaultMotionState* state = new btDefaultMotionState(
        btTransform(BtOgre::Convert::toBullet(GetNode()->GetRotation(Node::SCENE)),
                    BtOgre::Convert::toBullet(GetNode()->GetPosition(Node::SCENE))));

    mBody = new btRigidBody(mass, state, mCollisionShape, inertia);

    // Store pointer to this PhysicsBodyComponent for later retrieval (for
    // collisions, for instance)
    mBody->setUserPointer((void *)(this));
}

void PhysicsBodyComponent::OnDestroy() {
    delete mBody->getMotionState();
    delete mBody;
    delete mCollisionShape;
}

void PhysicsBodyComponent::OnEnable() {
    if(mCollisionMaskInUse) // Special treatment due to Bullet internals
        GetNode()->GetScene()->GetPhysicsWorld()->GetBulletWorld()->addRigidBody(mBody, mCollisionGroup, mCollisionMask);
    else
        GetNode()->GetScene()->GetPhysicsWorld()->GetBulletWorld()->addRigidBody(mBody);
}

void PhysicsBodyComponent::OnDisable() {
    GetNode()->GetScene()->GetPhysicsWorld()->GetBulletWorld()->removeRigidBody(mBody);
}

void PhysicsBodyComponent::OnCollide(PhysicsBodyComponent* other_body) {
    emit Collided(other_body);
}

btRigidBody* PhysicsBodyComponent::GetRigidBody() {
    return mBody;
}

void PhysicsBodyComponent::ApplyCentralImpulse(const btVector3& impulse) {
    mBody->applyCentralImpulse(impulse);
}

void PhysicsBodyComponent::SetCentralForce(const btVector3& force) {
    mCentralForce = force;
}

void PhysicsBodyComponent::SetTorque(const btVector3& torque)
{
    mTorque = torque;
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

void PhysicsBodyComponent::SetRestrictRotation(const btVector3& restriction) {
    mBody->setAngularFactor(restriction);
}

void PhysicsBodyComponent::SetGravity(const btVector3& gravity) {
    mBody->setGravity(gravity);
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
    btVector3 inertia;
    mCollisionShape->calculateLocalInertia(mass, inertia);
    mBody->setMassProps(mass, inertia);
}

void PhysicsBodyComponent::SetCollisionShapeType(CollisionShapeType type) {
    mCollisionShapeType = type;
}

}
