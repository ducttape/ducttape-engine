
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
      mBtGhostObject(nullptr),
      mMouseEnabled(true),
      mMouseSensitivity(1.0),
      mMouseYInversed(false),
      mMove(0.0, 0.0, 0.0),
      mMoveSpeed(5.0f),
      mWASDEnabled(true),
      mArrowsEnabled(true),
      mJumpEnabled(true) {}

void FPSPlayerComponent::OnCreate() {
    btTransform  start_trans;
    start_trans.setIdentity();
    start_trans.setOrigin(BtOgre::Convert::toBullet(GetNode()->GetPosition(Node::SCENE)));
    start_trans.setRotation(BtOgre::Convert::toBullet(GetNode()->GetRotation(Node::SCENE)));

    btScalar character_height = 1.75;
    btScalar character_width = 0.44;
    btConvexShape* capsule = new btCapsuleShape(character_width, character_height);

    mBtGhostObject = new btPairCachingGhostObject();
    mBtGhostObject->setWorldTransform(start_trans);
    mBtGhostObject->setCollisionShape(capsule);
    mBtGhostObject->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);
    btGhostPairCallback* ghost_pair_callback = new btGhostPairCallback();
    mBtGhostObject->setUserPointer((void *)this);

    mBtController = new btKinematicCharacterController(mBtGhostObject, capsule, 0);
    GetNode()->GetScene()->GetPhysicsWorld()->GetBulletWorld()->addCollisionObject(mBtGhostObject,btBroadphaseProxy::CharacterFilter, btBroadphaseProxy::StaticFilter | btBroadphaseProxy::DefaultFilter);
    GetNode()->GetScene()->GetPhysicsWorld()->GetBulletWorld()->addAction(mBtController);
    GetNode()->GetScene()->GetPhysicsWorld()->GetBulletWorld()->getPairCache()->setInternalGhostPairCallback(ghost_pair_callback);

    if(!QObject::connect(InputManager::Get(), SIGNAL(sKeyPressed(const OIS::KeyEvent&)), 
        this, SLOT(_HandleKeyPressed(const OIS::KeyEvent&)))) {
            Logger::Get().Error("Cannot connect the key pressed signal with " + GetName()
                + "'s keyboard input handling slot.");
    }
    if(!QObject::connect(InputManager::Get(), SIGNAL(sKeyReleased(const OIS::KeyEvent&)), 
        this, SLOT(_HandleKeyReleased(const OIS::KeyEvent&)))) {
            Logger::Get().Error("Cannot connect the key released signal with " + GetName()
                + "'s keyboard input handling slot.");
    }
    if(!QObject::connect(InputManager::Get(), SIGNAL(sMouseMoved(const OIS::MouseEvent&)), 
        this, SLOT(_HandleMouseInput(const OIS::MouseEvent&)))) {
            Logger::Get().Error("Cannot connect the mouse moved signal with " + GetName()
                + "'s mouse input handling slot.");
    }
}

void FPSPlayerComponent::OnDestroy() {
    if(mBtController)
        delete mBtController;
    if(mBtGhostObject)
        delete mBtGhostObject;
}

void FPSPlayerComponent::OnEnable() {
    SetWASDEnabled(true);
    SetArrowsEnabled(true);
    SetJumpEnabled(true);
}

void FPSPlayerComponent::OnDisable() {
    SetWASDEnabled(false);
    SetArrowsEnabled(false);
    SetJumpEnabled(false);
}

void FPSPlayerComponent::OnUpdate(double time_diff) {
    static Ogre::Vector3 move;
    static Ogre::Quaternion quaternion;
    static btTransform trans;

    quaternion = Ogre::Quaternion(GetNode()->GetRotation().getYaw(), Ogre::Vector3(0.0, 1.0, 0.0));
    move = quaternion * BtOgre::Convert::toOgre(mMove) * mMoveSpeed;
    mBtController->setVelocityForTimeInterval(BtOgre::Convert::toBullet(move), 0.5);

    trans = mBtGhostObject->getWorldTransform();

    GetNode()->SetPosition(BtOgre::Convert::toOgre(trans.getOrigin()), Node::SCENE);
}

void FPSPlayerComponent::SetWASDEnabled(bool wasd_enabled) {
    mWASDEnabled = wasd_enabled;
}

bool FPSPlayerComponent::GetWASDEnabled() const {
    return mWASDEnabled;
}

void FPSPlayerComponent::SetArrowsEnabled(bool arrows_enabled) {
    mArrowsEnabled = arrows_enabled;
}

bool FPSPlayerComponent::GetArrowsEnabled() const {
    return mArrowsEnabled;
}

void FPSPlayerComponent::SetMoveSpeed(float move_speed) {
    mMoveSpeed = move_speed;
}

float FPSPlayerComponent::GetMoveSpeed() const {
    return mMoveSpeed;
}

void FPSPlayerComponent::SetMouseEnabled(bool mouse_enabled) {
    mMouseEnabled = mouse_enabled;
}

bool FPSPlayerComponent::GetMouseEnabled() const {
    return mMouseEnabled;
}

void FPSPlayerComponent::SetMouseSensitivity(float mouse_sensitivity) {
    mMouseSensitivity = mouse_sensitivity;
}

float FPSPlayerComponent::GetMouseSensitivity() const {
    return mMouseSensitivity;
}

void FPSPlayerComponent::SetMouseYInversed(bool mouse_y_inversed) {
    mMouseYInversed = mouse_y_inversed;
}

bool FPSPlayerComponent::GetMouseYInversed() const {
    return mMouseYInversed;
}

void FPSPlayerComponent::SetJumpEnabled(bool jump_enabled) {
    mJumpEnabled = jump_enabled;
}

bool FPSPlayerComponent::GetJumpEnabled() const{
    return mJumpEnabled;
}

void FPSPlayerComponent::_HandleKeyPressed(const OIS::KeyEvent& event) {
    if(mWASDEnabled || mArrowsEnabled) {
        if((event.key == OIS::KC_W && mWASDEnabled) || (event.key == OIS::KC_UP && mArrowsEnabled)) {
            mMove.setZ(mMove.getZ() - 1.0f);
        }
        if((event.key == OIS::KC_S && mWASDEnabled) || (event.key == OIS::KC_DOWN && mArrowsEnabled)) {
            mMove.setZ(mMove.getZ() + 1.0f);
        }
        if((event.key == OIS::KC_A && mWASDEnabled) || (event.key == OIS::KC_LEFT && mArrowsEnabled)) {
            mMove.setX(mMove.getX() - 1.0f);
        }
        if((event.key == OIS::KC_D && mWASDEnabled) || (event.key == OIS::KC_RIGHT && mArrowsEnabled)) {
            mMove.setX(mMove.getX() + 1.0f);
        }
    }
    if(mJumpEnabled && event.key == OIS::KC_SPACE) {
        mBtController->jump();
    }
}

void FPSPlayerComponent::_HandleMouseInput(const OIS::MouseEvent& event) {
    if(mMouseEnabled) {
        float factor = mMouseSensitivity * -0.01;

        float dx = event.state.X.rel * factor;
        float dy = event.state.Y.rel * factor * (mMouseYInversed ? -1 : 1);

        if(dx != 0 || dy != 0) {
            // watch out for da gimbal lock !!

            Ogre::Matrix3 orientMatrix;
            GetNode()->GetRotation().ToRotationMatrix(orientMatrix);

            Ogre::Radian yaw, pitch, roll;
            orientMatrix.ToEulerAnglesYXZ(yaw, pitch, roll);

            pitch += Ogre::Radian(dy);
            yaw += Ogre::Radian(dx);

            // do not let it look completely vertical, or the yaw will break
            if(pitch > Ogre::Degree(89.9))
                pitch = Ogre::Degree(89.9);

            if(pitch < Ogre::Degree(-89.9))
                pitch = Ogre::Degree(-89.9);

            orientMatrix.FromEulerAnglesYXZ(yaw, pitch, roll);

            Ogre::Quaternion rot;
            rot.FromRotationMatrix(orientMatrix);
            GetNode()->SetRotation(rot);
        }
    }
}

void FPSPlayerComponent::_HandleKeyReleased(const OIS::KeyEvent& event) {
    if(mWASDEnabled || mArrowsEnabled) {
        if((event.key == OIS::KC_W && mWASDEnabled) || (event.key == OIS::KC_UP && mArrowsEnabled)) {
            mMove.setZ(mMove.getZ() + 1.0f);
        }
        if((event.key == OIS::KC_S && mWASDEnabled) || (event.key == OIS::KC_DOWN && mArrowsEnabled)) {
            mMove.setZ(mMove.getZ() - 1.0f);
        }
        if((event.key == OIS::KC_A && mWASDEnabled) || (event.key == OIS::KC_LEFT && mArrowsEnabled)) {
            mMove.setX(mMove.getX() + 1.0f);
        }
        if((event.key == OIS::KC_D && mWASDEnabled) || (event.key == OIS::KC_RIGHT && mArrowsEnabled)) {
            mMove.setX(mMove.getX() - 1.0f);
        }
    }
}

}