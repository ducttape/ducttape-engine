
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include <Logic/AdvancedPlayerComponent.hpp>

#include <Scene/Node.hpp>
#include <Scene/Scene.hpp>
#include <Logic/InteractionComponent.hpp>

#include <BtOgreGP.h>

namespace dt {

AdvancedPlayerComponent::AdvancedPlayerComponent(const QString& name)
    : Component(name),
      mBtController(nullptr),
      mBtGhostObject(nullptr),
      mMouseEnabled(true),
      mMouseSensitivity(1.0),
      mMouseYInversed(false),
      mMove(0.0, 0.0, 0.0),
      mMoveSpeed(5.0f),
      mKeyboardEnabled(true),
      mJumpEnabled(true),
      mIsLeftOneShot(true),
      mIsRightOneShot(true),
      mIsMoving(false),
      mIsLeftMouseDown(false),
      mIsRightMouseDown(false) {}

void AdvancedPlayerComponent::OnInitialize() {
    btTransform  start_trans;
    start_trans.setIdentity();
    start_trans.setOrigin(BtOgre::Convert::toBullet(GetNode()->GetPosition(Node::SCENE)));
    start_trans.setRotation(BtOgre::Convert::toBullet(GetNode()->GetRotation(Node::SCENE)));

    btScalar character_height = 1.75;
    btScalar character_width = 0.44;
    btConvexShape* capsule = new btCapsuleShape(character_width, character_height);

    mBtGhostObject = std::shared_ptr<btPairCachingGhostObject>(new btPairCachingGhostObject());
    mBtGhostObject->setWorldTransform(start_trans);
    mBtGhostObject->setCollisionShape(capsule);
    mBtGhostObject->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);
    /////////////////////////////////////////////////////////////////////////////////////////////////
    //TODO: Make a better way to distinguish between AdvancedPlayerComponent and PhysicsBodyComponent.
    //For now, it's just a null pointer check to do this.
    mBtGhostObject->setUserPointer(nullptr);
    /////////////////////////////////////////////////////////////////////////////////////////////////
    mBtController = std::shared_ptr<btKinematicCharacterController>
        (new btKinematicCharacterController(mBtGhostObject.get(), capsule, 1));
    GetNode()->GetScene()->GetPhysicsWorld()->GetBulletWorld()->addCollisionObject(mBtGhostObject.get());
    GetNode()->GetScene()->GetPhysicsWorld()->GetBulletWorld()->addAction(mBtController.get());

    if(!QObject::connect(InputManager::Get(), SIGNAL(sPressed(dt::InputManager::InputCode, const OIS::EventArg&)), 
                                        this, SLOT(_HandleButtonDown(dt::InputManager::InputCode, const OIS::EventArg&)))) {
            Logger::Get().Error("Cannot connect signal sPressed with " + GetName()
                + "'s input handling slot.");
    }
    if(!QObject::connect(InputManager::Get(), SIGNAL(sReleased(dt::InputManager::InputCode, const OIS::EventArg&)), 
                                        this, SLOT(_HandleButtonUp(dt::InputManager::InputCode, const OIS::EventArg&)))) {
            Logger::Get().Error("Cannot connect signal sReleased with " + GetName()
                + "'s input handling slot.");
    }
    if(!QObject::connect(InputManager::Get(), SIGNAL(sMouseMoved(const OIS::MouseEvent&)), 
                                        this, SLOT(_HandleMouseMove(const OIS::MouseEvent&)))) {
            Logger::Get().Error("Cannot connect signal sMouseMoved with " + GetName()
                + "'s input handling slot.");
    }
}

void AdvancedPlayerComponent::OnDeinitialize() {
    if(!QObject::disconnect(this, SLOT(_HandleButtonDown(dt::InputManager::InputCode, const OIS::EventArg&)))) {
            Logger::Get().Error("Cannot disconnect signal sPressed with " + GetName()
                + "'s input handling slot.");
    }
    if(!QObject::disconnect(this, SLOT(_HandleButtonUp(dt::InputManager::InputCode, const OIS::EventArg&)))) {
            Logger::Get().Error("Cannot disconnect signal sReleased with " + GetName()
                + "'s input handling slot.");
    }
    if(!QObject::disconnect(this, SLOT(_HandleMouseMove(const OIS::MouseEvent&)))) {
            Logger::Get().Error("Cannot disconnect signal sMouseMoved with " + GetName()
                + "'s input handling slot.");
    }
}

void AdvancedPlayerComponent::OnEnable() {
    SetKeyboardEnabled(true);
    SetMouseEnabled(true);

    //Re-sychronize it.
    btTransform transform;
    transform.setIdentity();
    transform.setOrigin(BtOgre::Convert::toBullet(GetNode()->GetPosition(Node::SCENE)));
    transform.setRotation(BtOgre::Convert::toBullet(GetNode()->GetRotation(Node::SCENE)));

    mBtGhostObject->setWorldTransform(transform);
    GetNode()->GetScene()->GetPhysicsWorld()->GetBulletWorld()->addCollisionObject(mBtGhostObject.get());
}

void AdvancedPlayerComponent::OnDisable() {
    SetKeyboardEnabled(false);
    SetMouseEnabled(false);

    GetNode()->GetScene()->GetPhysicsWorld()->GetBulletWorld()->removeCollisionObject(mBtGhostObject.get());
}

void AdvancedPlayerComponent::OnUpdate(double time_diff) {
    static Ogre::Vector3 move;
    static Ogre::Quaternion quaternion;
    static btTransform trans;

    quaternion = Ogre::Quaternion(GetNode()->GetRotation().getYaw(), Ogre::Vector3(0.0, 1.0, 0.0));
    move = quaternion * BtOgre::Convert::toOgre(mMove);
    move.normalise();
    move *= mMoveSpeed;
    mBtController->setVelocityForTimeInterval(BtOgre::Convert::toBullet(move), 0.5);

    trans = mBtGhostObject->getWorldTransform();

    GetNode()->SetPosition(BtOgre::Convert::toOgre(trans.getOrigin()), Node::SCENE);

    if(!move.isZeroLength() && mBtController->onGround() && !mIsMoving) {
        //Emit sMove when the player starts to move when he's stopping or has done jumpping.
        emit sMove();
        mIsMoving = true;
    }
    else if((move.isZeroLength() || !mBtController->onGround()) && mIsMoving) {
        //Emit sStop when the player starts to stop or jump while he's moving.
        emit sStop();
        mIsMoving = false;
    }

    if(mIsLeftMouseDown || mIsRightMouseDown) {
        _OnMouseTriggered();

        if(mIsLeftOneShot) {
            mIsLeftMouseDown = false;
        }
        if(mIsRightOneShot) {
            mIsRightMouseDown = false;
        }
    }

}

void AdvancedPlayerComponent::SetKeyboardEnabled(bool is_keyboard_enabled) {
    mKeyboardEnabled = is_keyboard_enabled;

    if(!mKeyboardEnabled)
        mMove.setZero();
}

bool AdvancedPlayerComponent::GetKeyboardEnabled() const {
    return mKeyboardEnabled;
}

void AdvancedPlayerComponent::SetMoveSpeed(float move_speed) {
    mMoveSpeed = move_speed;
}

float AdvancedPlayerComponent::GetMoveSpeed() const {
    return mMoveSpeed;
}

void AdvancedPlayerComponent::SetMouseEnabled(bool mouse_enabled) {
    mMouseEnabled = mouse_enabled;

    if(!mMouseEnabled) {
        mIsLeftMouseDown = false;
        mIsRightMouseDown = false;
    }
}

bool AdvancedPlayerComponent::GetMouseEnabled() const {
    return mMouseEnabled;
}

void AdvancedPlayerComponent::SetMouseSensitivity(float mouse_sensitivity) {
    mMouseSensitivity = mouse_sensitivity;
}

float AdvancedPlayerComponent::GetMouseSensitivity() const {
    return mMouseSensitivity;
}

void AdvancedPlayerComponent::SetMouseYInversed(bool mouse_y_inversed) {
    mMouseYInversed = mouse_y_inversed;
}

bool AdvancedPlayerComponent::GetMouseYInversed() const {
    return mMouseYInversed;
}

void AdvancedPlayerComponent::SetJumpEnabled(bool jump_enabled) {
    mJumpEnabled = jump_enabled;
}

bool AdvancedPlayerComponent::GetJumpEnabled() const{
    return mJumpEnabled;
}

void AdvancedPlayerComponent::_HandleButtonDown(dt::InputManager::InputCode input_code, const OIS::EventArg& event) {
    if(mKeyboardEnabled) {
        if(input_code == InputManager::KC_W || input_code == InputManager::KC_UP) {
            mMove.setZ(mMove.getZ() - 1.0f);
        }
        if(input_code == InputManager::KC_S || input_code == InputManager::KC_DOWN) {
            mMove.setZ(mMove.getZ() + 1.0f);
        }
        if(input_code == InputManager::KC_A || input_code == InputManager::KC_LEFT) {
            mMove.setX(mMove.getX() - 1.0f);
        }
        if(input_code == InputManager::KC_D || input_code == InputManager::KC_RIGHT) {
            mMove.setX(mMove.getX() + 1.0f);
        }

        if(mJumpEnabled && input_code == InputManager::KC_SPACE && mBtController->onGround()) {
            mBtController->jump();

            emit sStop();
            emit sJump();
        }
    }

    if(mMouseEnabled) {
        if(input_code == InputManager::MC_LEFT) {
            mIsLeftMouseDown = true;
        }
        else if(input_code == InputManager::MC_RIGHT) {
            mIsRightMouseDown = true;
        }
    }
}

void AdvancedPlayerComponent::_HandleMouseMove(const OIS::MouseEvent& event) {
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

void AdvancedPlayerComponent::_HandleButtonUp(dt::InputManager::InputCode input_code, const OIS::EventArg& event) {
    if(mKeyboardEnabled) {
        if(input_code == InputManager::KC_W || input_code == InputManager::KC_UP) {
            mMove.setZ(mMove.getZ() + 1.0f);
        }
        if(input_code == InputManager::KC_S || input_code == InputManager::KC_DOWN) {
            mMove.setZ(mMove.getZ() - 1.0f);
        }
        if(input_code == InputManager::KC_A || input_code == InputManager::KC_LEFT) {
            mMove.setX(mMove.getX() + 1.0f);
        }
        if(input_code == InputManager::KC_D || input_code == InputManager::KC_RIGHT) {
            mMove.setX(mMove.getX() - 1.0f);
        }
    }

    if(mMouseEnabled) {
        if(input_code == InputManager::MC_LEFT) {
            mIsLeftMouseDown = false;
        }
        else if(input_code == InputManager::MC_RIGHT) {
            mIsRightMouseDown = false;
        }
    }
}

bool AdvancedPlayerComponent::GetIsOneShot(OIS::MouseButtonID mouse_button) const {
    if(mouse_button == OIS::MB_Left) {
        return mIsLeftOneShot;
    }
    else {
        return mIsRightOneShot;
    }
}

void AdvancedPlayerComponent::SetIsOneShot(bool is_one_shot, OIS::MouseButtonID mouse_button) {
    if(mouse_button == OIS::MB_Left) {
        mIsLeftOneShot = is_one_shot;
    }
    else {
        mIsRightOneShot = is_one_shot;
    }
}

}
