
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include "utils/Logger.hpp"

#include "PhysicsBodyComponent.hpp"

namespace dt {

PhysicsBodyComponent::PhysicsBodyComponent(const std::string& mesh_component_name,
                                           const std::string& name)
    : Component(name) {
    mMeshComponentName = mesh_component_name;
}

void PhysicsBodyComponent::HandleEvent(std::shared_ptr<Event> e) {
    if(e->GetType() == "DT_FREEZEPHYSICS") {}
    if(e->GetType() == "DT_UNFREEZEPHYSICS") {}
    if(e->GetType() == "DT_CHANGEGRAVITY") {
        std::shared_ptr<ChangeGravityEvent> g =
            std::dynamic_pointer_cast<ChangeGravityEvent>(e);
        g->GetDirection();
    }
}

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
    mCollisionShape = converter.createSphere();

    btScalar mass = 5;
    btVector3 inertia;
    mCollisionShape->calculateLocalInertia(mass, inertia);

    mMotionState = new BtOgre::RigidBodyState(mesh_component->GetOgreSceneNode());

    mBody = new btRigidBody(mass, mMotionState, mCollisionShape, inertia);

    PhysicsManager::Get()->GetPhysicsWorld()->addRigidBody(mBody);
}

void PhysicsBodyComponent::OnDestroy() {}

void PhysicsBodyComponent::OnUpdate(double time_diff) {
    btTransform trans;
    mBody->getMotionState()->getWorldTransform(trans);
    std::cout << "x: " << trans.getOrigin().getX() << " y: "
        << trans.getOrigin().getY() << " z: " << trans.getOrigin().getZ() << std::endl;
}

}
