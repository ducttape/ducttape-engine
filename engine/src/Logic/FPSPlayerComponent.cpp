
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include <Logic/FPSPlayerComponent.hpp>

#include <Scene/Node.hpp>
#include <Scene/Scene.hpp>
#include <Input/InputManager.hpp>

#include <BtOgreGP.h>

namespace dt {

FPSPlayerComponent::FPSPlayerComponent(const QString& name)
    : mBtController(nullptr),
      mBtGhostObject(nullptr) {}

void FPSPlayerComponent::OnCreate() {
    btTransform  start_trans;
    start_trans.setIdentity();
    start_trans.setOrigin(BtOgre::Convert::toBullet(GetNode()->GetPosition(Node::SCENE)));
    start_trans.setRotation(BtOgre::Convert::toBullet(GetNode()->GetRotation(Node::SCENE)));

    btScalar character_height = 1.75;
    btScalar character_width = 1.75;
    btConvexShape* capsule = new btCapsuleShape(character_width, character_height);

    mBtGhostObject = new btPairCachingGhostObject();
    mBtGhostObject->setWorldTransform(start_trans);
    mBtGhostObject->setCollisionShape(capsule);
    mBtGhostObject->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);
    mBtGhostObject->setUserPointer((void *)this);

    mBtController = new btKinematicCharacterController(mBtGhostObject, capsule, 0.35);
    GetNode()->GetScene()->GetPhysicsWorld()->GetBulletWorld()->addCollisionObject(mBtGhostObject,btBroadphaseProxy::CharacterFilter, btBroadphaseProxy::StaticFilter|btBroadphaseProxy::DefaultFilter);
    GetNode()->GetScene()->GetPhysicsWorld()->GetBulletWorld()->addAction(mBtController);

    if(!QObject::connect(InputManager::Get(), SIGNAL(sKeyPressed(const OIS::KeyEvent&)), 
        this, SLOT(_HandleKeyPressed(const OIS::KeyEvent&)))) {
            Logger::Get().Error("Cannot connect the key pressed signal with " + GetName()
                + "'s keyboard input handling slot.");
    }
}

void FPSPlayerComponent::OnDestroy() {
    if(mBtController)
        delete mBtController;
    if(mBtGhostObject)
        delete mBtGhostObject;
}

void FPSPlayerComponent::OnEnable() {}

void FPSPlayerComponent::OnDisable() {}

void FPSPlayerComponent::OnUpdate(double time_diff) {
    btTransform trans;
    trans = mBtGhostObject->getWorldTransform();

    GetNode()->SetPosition(BtOgre::Convert::toOgre(trans.getOrigin()), Node::SCENE);
    GetNode()->SetRotation(BtOgre::Convert::toOgre(trans.getRotation()), Node::SCENE);
}

void FPSPlayerComponent::_HandleKeyPressed(const OIS::KeyEvent& event) {
    /*btTransform trans;
    trans.setIdentity();
    btVector3 vec(0.0f, 0.0f, 0.0f);

    if(event.key == OIS::KC_W) {
        vec.setZ(-1.0f);

    }
    if(event.key == OIS::KC_S) {
        vec.setZ(1.0f);
    }
    if(event.key == OIS::KC_A) {
        vec.setX(-1.0f);
    }
    if(event.key == OIS::KC_D) {
        vec.setX(1.0f);
    }
    if(event.key == OIS::KC_SPACE) {
        mBtController->jump();
    }

    trans.setOrigin(vec);*/
    //mBtGhostObject->setWorldTransform(trans * mBtGhostObject->getWorldTransform());
}

}